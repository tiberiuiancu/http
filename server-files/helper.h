#ifndef PROJECT_HELPER_H
#define PROJECT_HELPER_H

#include <vector>
#include <string>

void red();
void green();
void white();
void log(int status);
std::vector<std::string> split(std::string s, const char *c);
std::vector<std::string> split(std::string s, const char *c, int cnt);
char* getDate();

#define STATUS_SUCCESS 1
#define STATUS_FAILURE -1
#define STATUS_NEUTRAL 0

#define CRLF "\r\n"
#define SP " "
#define HTAB "\t"
#define HTTP_VERSION HTTP/1.1


#endif //PROJECT_HELPER_H
