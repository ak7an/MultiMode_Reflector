#pragma once
#include <string>

enum LogLevel { INFO, WARN, ERROR };

class Logger {
public:
    static void init(const std::string& file);
    static void log(LogLevel level, const std::string& msg);
};
