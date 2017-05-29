#ifndef TIN_LOGGING_H
#define TIN_LOGGING_H

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <sstream>

enum LogLevel {
    ll_info, ll_debug, ll_error, ll_warn
};

inline const char *level_description(LogLevel level) {
    switch (level) {
        case ll_info:
            return " INFO: ";
        case ll_debug:
            return "DEBUG: ";
        case ll_error:
            return "ERROR: ";
        case ll_warn:
            return " WARN: ";
    }
}

class Logger {
private:
    std::stringstream ss;
    LogLevel level;

public:
    Logger(LogLevel level) : level(level) {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        ss << level_description(level)
           << std::put_time(&tm, "%d.%m.%Y %H:%M:%S")
           << " | ";
    }
    Logger(const Logger& other) : ss(other.ss.str()), level(other.level) {}

    template<typename T>
    Logger &operator<<(T data) {
        ss << data;
        return *this;
    }

    ~Logger() {
        ss << std::endl;
        auto stream = level == ll_error ? &std::cerr : &std::cout;
        *stream << ss.str();
    }
};

inline Logger log(LogLevel level = LogLevel::ll_info) {
    return Logger(level);
}

inline Logger logInfo() {
    return log(ll_info);
}

inline Logger logDebug() {
    return log(ll_debug);
}

inline Logger logError() {
    return log(ll_error);
}

inline Logger logWarn() {
    return log(ll_warn);
}

#endif //TIN_LOGGING_H
