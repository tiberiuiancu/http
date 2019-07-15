#include "helper.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <time.h>

void red() {
    std::cout << "\033[31m";
}

void green() {
    std::cout << "\033[32m";
}

void white() {
    std::cout << "\033[37m";
}

void log(int status) {
    std::cout << "[";
    if (status == STATUS_FAILURE) {
        red();
        std::cout << "+";
        white();
    } else if (status == STATUS_SUCCESS) {
        green();
        std::cout << "+";
        white();
    } else {
        white();
        std::cout << "+";
    }
    std::cout << "]";
}

std::vector<std::string> split(std::string s, const char *c) {
    std::vector<std::string> res;

    int len = strlen(c);

    int last = 0;
    for (int it = 0; it <= s.size() - len; ++it) {
        if (s.compare(it, len, c) == 0) {
            std::string newStr = s.substr(last, it - last);
            res.push_back(newStr);
            it += len - 1;
            last = it + 1;
        }
    }

    res.push_back(s.substr(last, s.size() - last));

    return res;
}

std::vector<std::string> split(std::string s, const char *c, int cnt) {
    std::vector<std::string> res;

    int len = strlen(c);

    int last = 0;
    for (int it = 0; it <= s.size() - len && cnt; ++it) {
        if (s.compare(it, len, c) == 0) {
            std::string newStr = s.substr(last, it - last);
            res.push_back(newStr);
            cnt--;
            it += len - 1;
            last = it + 1;
        }
    }

    res.push_back(s.substr(last, s.size() - last));

    return res;
}

char* getDate() {
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);

    char* aux = (char*)malloc(sizeof(buf));
    strcpy(aux, buf);
    return aux;
}

