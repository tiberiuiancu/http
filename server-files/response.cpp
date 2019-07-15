#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <string>

#include "response.h"
#include "helper.h"


int time() {
    return (int)std::time(nullptr);
}

void handleRequest(int socketfd) {
    // make socket non-blocking
    fcntl(socketfd, F_SETFL, O_NONBLOCK);

    char *buffer = (char*)malloc(REQUEST_LENGTH + 1);
    int currentTime = 0, lastTime = time();

    while (currentTime - lastTime < TIMEOUT) {
        int res = recv(socketfd, buffer, REQUEST_LENGTH, 0);
        if (res == REQUEST_LENGTH) {
            if (recv(socketfd, NULL, REQUEST_LENGTH, 0) > 0) {
                char *response = getResponse("494 Request header too large");
                send(socketfd, response, strlen(response), 0);
                free(response);
                break;
            }
        }

        if (res < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                currentTime = time();
                std::cout << "no data to read in recv\n";
                continue;
            }
            log(STATUS_FAILURE);
            std::cout << "recv error\n";
            break;
        } else if (res == 0) {
            log(STATUS_NEUTRAL);
            std::cout << "client closed connection\n";
            break;
        }

        char *response = makeResponse(buffer);

        send(socketfd, response, strlen(response), 0);
        free(response);

        lastTime = time();
        currentTime = lastTime;
    }

    char *response = makeResponse("GET /hello.htm HTTP/1.1\r\nDate: 123");

    if (currentTime - lastTime > TIMEOUT) {
        char *response = getResponse("408 Request Timeout");
        send(socketfd, response, strlen(response), 0);
        free(response);
    }

    // terminate connection
    log(STATUS_NEUTRAL);
    std::cout << "socket closed\n";
    recv(socketfd, NULL, REQUEST_LENGTH, 0);
    shutdown(socketfd, 2);
    free(buffer);
}

char* makeResponse(char *request) {
    std::vector<std::string> lines = split(request, CRLF);

    // parse the request line
    std::vector <std::string> requestLine;

    try {
        requestLine = split(lines[0], SP);
        if (requestLine.size() != 3)
            return getResponse("400 Bad Request");
    } catch (int err) {
        return getResponse("400 Bad Request");
    }

    if (requestLine[0].compare("GET") == 0) {
        try {
            if (access(requestLine[1].c_str(), F_OK) == -1) {
                char *ptr = getResponse("404 Not Found");

                return ptr;
            }
            freopen(requestLine[1].c_str(), "r", stdin);
            char *buf = (char*)malloc(MAX_FILE_SIZE + 1);
            std::cin.get(buf, MAX_FILE_SIZE);
            fclose(stdin);

            char *response = getResponse("200 OK");
            char *aux = (char*)malloc(sizeof(response) + sizeof(buf) + 5 * sizeof(char));
            strcpy(aux, response);
            strcat(aux, CRLF);
            strcat(aux, buf);

            free(buf);

            return aux;
        } catch (int err) {
            return getResponse("404 Not Found");
        }
    } else {
        std::vector<std::string> headers;
        headers.push_back(std::string("Allow: GET"));
        return getResponse("405 Method Not Allowed", headers);
    }
}

char* getResponse(char *response) {
    std::string fullResponse;
    fullResponse += "HTTP/1.1";
    fullResponse += SP;
    fullResponse += response;
    fullResponse += CRLF;
    fullResponse = appendHeaders(fullResponse);

    char *aux = (char*)malloc(fullResponse.length() + 1);
    strcpy(aux, fullResponse.c_str());

    return aux;
}

char* getResponse(char *response, std::vector<std::string> headers) {
    std::string fullResponse;
    fullResponse += "HTTP/1.1";
    fullResponse += SP;
    fullResponse += response;
    fullResponse += CRLF;
    for (int i = 0; i < headers.size(); ++i) {
        fullResponse += headers[i];
    }
    fullResponse = appendHeaders(fullResponse);

    char *aux = (char*)malloc(sizeof(fullResponse.c_str()));
    strcpy(aux, fullResponse.c_str());
    return aux;
}

std::string appendHeaders(std::string response) {
    // server header
    std::string server("Server: RimRecker6.7");
    // date header
    char *auxDate = getDate();
    std::string date("Date: ");
    date += auxDate;
    free(auxDate);

    response += server;
    response += CRLF;
    response += date;
    response += CRLF;

    return response;
}

