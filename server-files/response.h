#ifndef PROJECT_RESPONSE_H
#define PROJECT_RESPONSE_H

#define REQUEST_LENGTH 8192 // bytes
#define MAX_FILE_SIZE 10 * 1024 * 1024 // bytes (10 megabytes)
#define TIMEOUT 1 // seconds

#include <vector>
#include <string>

char* makeResponse(char *request);
void handleRequest(int socketfd);
char* getResponse(char *response);
char* getResponse(char *response, std::vector<std::string> headers);
std::string appendHeaders(std::string response);


#endif //PROJECT_RESPONSE_H
