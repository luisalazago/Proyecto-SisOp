# This is a code to test the concurrence of the server.

import requests

def main():
    n = int(input("Put an N: "))
    URL = "http://localhost:9090/"
    k = 1
    for i in range(n):
        response = requests.get(URL)
        print("You made a request.")
    print("\nYou just finished the requests.")

main()
