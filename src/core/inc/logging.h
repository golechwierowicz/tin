#ifndef TIN_LOGGING_H
#define TIN_LOGGING_H

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstring>

enum LogLevel { ll_info, ll_debug, ll_error, ll_warn };

template <typename T>
void log(T& data, LogLevel level = LogLevel::ll_info) {
    const char* levelDescr;
    switch (level) {
    case LogLevel::ll_info:
        levelDescr = " INFO: ";
        break;
    case LogLevel::ll_debug:
        levelDescr = "DEBUG: ";
        break;
    case LogLevel::ll_error:
        levelDescr = "ERROR: ";
        break;
    case LogLevel::ll_warn:
        levelDescr = " WARN: ";
        break;
    }

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::cout << levelDescr << std::put_time(&tm, "%d.%m.%Y %H:%M:%S") << " | " <<  data << std::endl;
}

template <typename T>
void log_info(T& data) {
    log(data, LogLevel::ll_info);
}

template <typename T>
void log_debug(T& data) {
    log(data, LogLevel::ll_debug);
}

template <typename T>
void log_error(T& data) {
    log(data, LogLevel::ll_error);
}

template <typename T>
void log_warn(T& data) {
    log(data, LogLevel::ll_warn);
}

inline void log_exception(const std::exception& e) {
    const char* error_message = e.what();
    log_error(error_message);
}

inline void log_errno() {
    const char* error_message = strerror(errno);
    log_error(error_message);
}


#endif //TIN_LOGGING_H
