#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <fcntl.h>

#include "response.h"
#include "helper.h"

#define PORT 30000
#define IP_ADDRESS "127.0.0.1"
#define LISTEN_BACKLOG 5


#define SLEEPINTERVAL 1000 * 10


int main(int argc, char *argv[]) {

    uid_t uid = getuid(), euid = geteuid();
    if (PORT < 1024 && (uid != 0 || euid != uid)) {
        log(STATUS_FAILURE);
        std::cout << "WARNING: program must be ran as root to bind on ports lower than 1024; current port: " << PORT << "\n";
    }

	// create socket file descriptor
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0) {
		log(STATUS_FAILURE);
		std::cout << "failed to create socket";
		exit(EXIT_FAILURE);
	} else {
	    log(STATUS_SUCCESS);
	    std::cout << "socket created successfully\n";
	}

	// bind socket to address
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, IP_ADDRESS, &addr.sin_addr.s_addr) <= 0) {
        log(STATUS_FAILURE);
        std::cout << "ip address invalid";
        exit(EXIT_FAILURE);
    }

	if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        log(STATUS_FAILURE);
        std::cout << "bind failed";
//	    perror("bind");
        exit(EXIT_FAILURE);
	} else {
	    log(STATUS_SUCCESS);
	    std::cout << "socket bound at " << IP_ADDRESS << ":" << PORT << "\n";
	}

	if (listen(socketfd, LISTEN_BACKLOG) < 0) {
        log(STATUS_FAILURE);
        std::cout << "listening failed";
	    exit(EXIT_FAILURE);
	} else {
	    log(STATUS_SUCCESS);
	    std::cout << "socket started listening for max " << LISTEN_BACKLOG << " connections\n";
	}

	while (true) {
        int newSocket = accept(socketfd, NULL, NULL);

        if (newSocket < 0) {
            log(STATUS_FAILURE);
            std::cout << "failed to accept connection\n";
            continue;
        }
        log(STATUS_SUCCESS);
        std::cout << "connection established with " << newSocket << "\n";

        // create new thread for the connection
//        std::thread t(handleRequest, newSocket);
//        t.detach();
	    handleRequest(newSocket);
		usleep(SLEEPINTERVAL);
	}
}
