#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define PORT 30000
#define IP_ADDRESS "127.0.0.1"
#define LENGTH 8192


int main(int argc, char *argv[]) {

    char *hello = "GET /hello.htm HTTP/1.1\r\nDate: 123";

    // create socket file descriptor
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        std::cout << "failed to create socket";
        exit(EXIT_FAILURE);
    } else {
        std::cout << "socket created successfully\n";
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP_ADDRESS, &server_addr.sin_addr.s_addr) <= 0) {
        std::cout << "ip address invalid";
        exit(EXIT_FAILURE);
    }

    connect(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send(socketfd, hello, strlen(hello), 0);
    std::cout << "hello sent\n";

    char *buffer = new char[LENGTH];
    int rc = recv(socketfd, buffer, LENGTH, 0);
    if (rc < 0) {
        perror("recv");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        std::cout << "conn closed";
    }

    std::cout << buffer;
    shutdown(socketfd, 2);
    return 0;
}
