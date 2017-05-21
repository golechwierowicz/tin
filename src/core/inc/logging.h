#ifndef TIN_LOGGING_H
#define TIN_LOGGING_H

#include <iostream>
#include <iomanip>
#include <ctime>

enum LogLevel { info, debug, error, warn };

template <typename T>
void log(T& data, LogLevel level = LogLevel::info) {
    const char* levelDescr;
    switch (level) {
    case LogLevel::info:
        levelDescr = " INFO: ";
        break;
    case LogLevel::debug:
        levelDescr = "DEBUG: ";
        break;
    case LogLevel::error:
        levelDescr = "ERROR: ";
        break;
    case LogLevel::warn:
        levelDescr = " WARN: ";
        break;
    }

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::cout << levelDescr << std::put_time(&tm, "%d.%m.%Y %H:%M:%S") << " | " <<  data << std::endl;
}

template <typename T>
void log_info(T& data) {
    log(data, LogLevel::info);
}

void log_info(const char* str) {
    log(str, LogLevel::info);
}

template <typename T>
void log_debug(T& data) {
    log(data, LogLevel::debug);
}

void log_debug(const char* str) {
    log(str, LogLevel::debug);
}

template <typename T>
void log_error(T& data) {
    log(data, LogLevel::error);
}

void log_error(const char* str) {
    log(str, LogLevel::error);
}

template <typename T>
void log_warn(T& data) {
    log(data, LogLevel::warn);
}

void log_warn(const char* str) {
    log(str, LogLevel::warn);
}



#endif //TIN_LOGGING_H
