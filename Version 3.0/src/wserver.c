#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sqlite3.h>
#include "request.h"
#include "io_helper.h"
#include <pthread.h>

#define NAME "/dory"
#define NUM 3
#define lli long long int
#define SIZE (NUM * sizeof(int))
#define quatum_time 5

typedef struct {
	int i;
	int listen_fd;
} Arguments; //Argumentos que se pasan a la función del manejo de hilos.

char default_root[] = ".";

int lectura; //Flag para evitar el race condition
sqlite3 *db; //Base de datos

lli **data; //Datos a guardar en la memoria data[0] = pthread_t, data[1] = time init, data[2] = time_end
int *thread_state; //Estados de los hilos para FIFO.

static int callback(void *NotUsed, int argc, char **argv, char **azColNAme);

int insert_database();

void *server_manager(void* args); //Hilo encargado del server manager

void *http_thread(void* args); //Hilo creador de peticiones http para el scheduler FIFO.

void *http_thread1(void* args); //Hilo creador de peticiones http para el scheduler SJF.

int get_fd(int listen_fd);

int main(int argc, char *argv[]) {
    int c, THREAD = 1, i;
    char *root_dir = default_root;
    int port = 10000;
	lectura = 0;
	int rc = sqlite3_open("database.db", &db);
	int schedule;

	//Dinamic Values
	int* value_request;
	time_t* time_init;
	time_t* time_end;
	pthread_t* request;

	if(rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 0;
	}

    while ((c = getopt(argc, argv, "d:p:")) != -1)
	switch (c) {
	case 'd':
	    root_dir = optarg;
	    break;
	case 'p':
	    port = atoi(optarg);
	    break;
	default:
	    fprintf(stderr, "usage: wserver [-d basedir] [-p port] [-t threads] [-b buffers]\n");
	    exit(1);
	}

	printf("Mama mia %d\n", argc);
	schedule = 1;
	if(argc == 9) {
		sscanf(argv[6], "%d", &THREAD);

		if(!strcmp(argv[8], "SJF")) schedule = 1;
		else if(!strcmp(argv[8], "FIFO")) schedule = 0;
		else {
			printf("Error\n");
			fprintf(stderr, "usage: wserver [-d basedir] [-p port] [-t threads] [-b buffers] SJF or FIFO\n");
			exit(1);
		}
	}

	// sizing the data and the states of the threads

	data = (lli**) malloc(THREAD * sizeof(lli*));
	for(i = 0; i < THREAD; ++i) {
		data[i] = (lli*) malloc(NUM * sizeof(lli));
	}
	printf("hello pana %s %d\n", argv[6], THREAD);
	thread_state = (int*) malloc(THREAD * sizeof(int));
	value_request = (int*) malloc(THREAD * sizeof(int));
	time_init = (time_t*) malloc(THREAD * sizeof(time_t));
	time_end = (time_t*) malloc(THREAD * sizeof(time_t));
	request = (pthread_t*) malloc(THREAD * sizeof(pthread_t));
    // run out of this directory

    chdir_or_die(root_dir);
	
    // now, get to work
	pthread_t temp[1];
    int listen_fd = open_listen_fd_or_die(port);
	pthread_t manager;
	pthread_create(&manager, NULL, server_manager, (int *) &THREAD);

	/* Thread States:
	0 = recien creado,
	1 = ocupado,
	2 = acaba de terminar una tarea.
	*/

	while (1) {
		if(!lectura){
			int i, j, temp2, k;
			srand(time(NULL));
			for(i = 0; i < THREAD; ++i){
				//Creación de los hilos y sus estados.
				time_init[i] = time(NULL);
				if(!schedule) thread_state[i] = 0;
				else if(schedule) value_request[i] = rand() % 10;
			}
			if(!schedule) {
				for(i = 0; i < THREAD; ++i) {
					if(!thread_state[i]) {
						thread_state[i] = 1;
						Arguments ar;
						ar.i = i;
						ar.listen_fd = listen_fd; 
						pthread_create(&request[i], NULL, http_thread, (Arguments *) &ar);
						printf("\n###########################\n");
						while(thread_state[i] != 2) printf("Ejecutando Hilo\n");
						printf("\n###########################\n");
						pthread_join(request[i], NULL);
						time_end[i] = time(NULL);
						printf("Termine el hilo Hijo nro: ");
						printf(" %lld\n\n", request[i]);
						data[i][0] = (lli) request[i]; data[i][1] = (lli) time_init[i]; data[i][2] = (lli) time_end[i];
					}
				}
			}
			else if(schedule) {
				k = 0;
				while(k < 2) {
					for(i = 0; i < THREAD; ++i) {
						if(!k) {
							//Order the threads based on the value got random.
							for(j = 0; j < THREAD; ++j) {
								if(value_request[j] < value_request[i]) {
									temp[0] = request[i];
									request[i] = request[j];
									request[j] = temp[0];
									temp2 = value_request[i];
									value_request[i] = value_request[j];
									value_request[j] = temp2;
								}
							}
						}
						else {
							//Process the threads pre-ordered.
							pthread_create(&request[i], NULL, http_thread1, (int *) &listen_fd);
							printf("\n###########################\n");
							printf("Ejecutando Hilo\n");
							printf("\n###########################\n");
							pthread_join(request[i], NULL);
							time_end[i] = time(NULL);
							printf("Termine el hilo Hijo nro: ");
							printf(" %lld\n\n", request[i]);
							data[i][0] = (lli) request[i]; data[i][1] = (lli) time_init[i]; data[i][2] = (lli) time_end[i];
						}
					}
					k += 1;
				}
			}
			lectura = 1;
		}
	}
	sqlite3_close(db);

	// free the memory used
	for(i = 0; i < NUM; ++i) {
		free(data[i]);
	}
	free(data);
	
	free(thread_state);

	free(value_request);
	free(time_init);
	free(time_end);
	free(request);

    return 0;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColNAme){
	int i;
	for(i = 0; i <argc;++i){
		printf("%s = %s\n", azColNAme[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int insert_database(int i){
	char *err_msg = 0;
	int rc;
	char sql_i[500];
	sprintf(sql_i, "INSERT INTO Request (pid, time_init, time_end) VALUES(%lld, %lld, %lld);", data[i][0], data[i][1], data[i][2]);
	rc= sqlite3_exec(db, sql_i, callback, 0, &err_msg);
	printf("Estoy revisando el insert %d\n", rc);
	printf("Y el mensaj es %s\n", sql_i);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		return 1;
	}

	return 0;
}

void *server_manager(void* args){
	int THREAD = *((int*) args);
	while(1){
		if(lectura){
			printf("hay manito entre %d\n", THREAD);
			int i;
			for(i = 0; i < THREAD; ++i){
				if(!insert_database(i))
					printf("get data succesefull\n");
			}
			lectura = 0;
		}
	}
	pthread_exit(0);
}

void *http_thread(void* args) {
	int listen_fd = ((Arguments *) args)->listen_fd;
	int i = ((Arguments *) args)->i;
	printf("\nCree un Hilo hijo\n");
	struct sockaddr_in client_addr;
	int client_len = sizeof(client_addr);
	int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
	request_handle(conn_fd);
	close_or_die(conn_fd);
	thread_state[i] = 2;
	pthread_exit(0);
}

void *http_thread1(void* args) {
	int listen_fd = *((int*) args);
	printf("\nCree un Hilo hijo\n");
	struct sockaddr_in client_addr;
	int client_len = sizeof(client_addr);
	int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
	request_handle(conn_fd);
	close_or_die(conn_fd);
	pthread_exit(0);
}
