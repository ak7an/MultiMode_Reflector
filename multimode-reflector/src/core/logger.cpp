#include "logger.h"
#include <fstream>
#include <iostream>
#include <ctime>

static std::ofstream logFile;

void Logger::init(const std::string& file) {
    logFile.open(file, std::ios::app);
}

void Logger::log(LogLevel level, const std::string& msg) {

    const char* lvl =
        level == INFO ? "INFO" :
        level == WARN ? "WARN" : "ERROR";

    time_t now = time(nullptr);

    logFile << "[" << now << "][" << lvl << "] "
            << msg << std::endl;

    std::cout << "[" << lvl << "] "
              << msg << std::endl;
}
