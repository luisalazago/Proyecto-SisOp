#include <stdio.h>
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
#define SIZE (NUM * sizeof(int))
#define lli long long int
#define THREAD 10

char default_root[] = ".";

int lectura; //Flag para evitar el race condition
sqlite3 *db; //Base de datos

lli data[THREAD][3]; //Datos a guardar en la memoria data[0] = pthread_t, data[1] = time init, data[2] = time_end

static int callback(void *NotUsed, int argc, char **argv, char **azColNAme);

int insert_database();

void *server_manager(void* args); //Hilo encargado del server manager

void *http_thread(void* args); //Hilo creador de las peticiones http;

// int share_memory(pid_t pid, time_t time_init, time_t time_end){
// 	int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
// 	if(fd < 0)
// 		return EXIT_FAILURE;
	
// 	ftruncate(fd, SIZE);

// 	int *data = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
// 	data[0] = (int)pid; data[1] = (int)time_init; data[2] = (int)time_end;

// 	munmap(data, SIZE);
// 	close(fd);
// 	return EXIT_SUCCESS;
// }

// int get_memory(sqlite3 *db){
// 	int fd = shm_open(NAME, O_RDONLY, 0666);
// 	if(fd < 0)
// 		return EXIT_FAILURE;
	
// 	int *data = (int *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
// 	//Codigo para ponerlo en sqllite
// 	if(insert_database(data, db))
// 		printf("Error al cargar la base de datos\n");

// 	munmap(data, SIZE);
// 	close(fd);
// 	shm_unlink(NAME);
// 	return EXIT_SUCCESS;
// }
//
// ./wserver [-d <basedir>] [-p <portnum>] 
// 

int main(int argc, char *argv[]) {
    int c;
    char *root_dir = default_root;
    int port = 10000;
	lectura = 0;
	int rc = sqlite3_open("database.db", &db);

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
	    fprintf(stderr, "usage: wserver [-d basedir] [-p port]\n");
	    exit(1);
	}

    // run out of this directory
    chdir_or_die(root_dir);

	// now, open conection to the database
	

    // now, get to work
	time_t time_init[THREAD];
	time_t time_end[THREAD];
	pthread_t request[THREAD];
    int listen_fd = open_listen_fd_or_die(port);
	pthread_t manager;
	pthread_create(&manager, NULL, server_manager, NULL);

	while (1) {
		if(!lectura){
			int i;
			//Punto 2) crear un proceso que se encargue de una petición http
			for(i = 0; i < THREAD; ++i){
			//Creación de los procesos htpps que llegan
				time_init[i] = time(NULL);
				pthread_create(&request[i], NULL, http_thread, (int *) &listen_fd);
			}
			//Proceso padre
			//Creación de los procesos htpps que llegan
			//Proceso padre
			for(i = 0; i < THREAD; ++i){
				pthread_join(request[i], NULL);
				time_end[i] = time(NULL);
				printf("Termine el proceso Hijo nro: ");
				printf(" %lld\n\n", request[i]);
				data[i][0] = (lli) request[i]; data[i][1] = (lli) time_init[i]; data[i][2] = (lli) time_end[i];
			}
			lectura = 1;
		}
	}
	sqlite3_close(db);
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

	// printf("Estoy revisando la creacion %d\n", rc);
	// if(rc != SQLITE_OK) {
	// 	fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
	// 	sqlite3_close(db);
	// 	return 1;
	// }

	// char *sql_c = "CREATE TABLE Request(pid INT, time_init INT, time_end INT);";
	
	// rc= sqlite3_exec(db, sql_c, 0, 0, &err_msg);
	// printf("Estoy revisando la Tablar %d\n", rc);
	// if(rc != SQLITE_OK) {
	// 	fprintf(stderr, "Cannot create database: %s\n", err_msg);
	// 	sqlite3_close(db);
	// 	return 1;
	// }

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
	while(1){
		if(lectura){
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

void *http_thread(void* args){
	int listen_fd = *((int *) args);
	printf("\nCree un proceso hijo\n");
	struct sockaddr_in client_addr;
	int client_len = sizeof(client_addr);
	int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
	request_handle(conn_fd);
	close_or_die(conn_fd);
	pthread_exit(0);
}