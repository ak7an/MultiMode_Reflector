#include "config.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

std::string Config::trim(const std::string& s) {

    size_t start = 0;
    while (start < s.size() &&
           std::isspace(static_cast<unsigned char>(s[start]))) {
        start++;
    }

    size_t end = s.size();
    while (end > start &&
           std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        end--;
    }

    return s.substr(start, end - start);
}

bool Config::load(const std::string& file) {

    std::ifstream in(file);

    if (!in.is_open()) {
        return false;
    }

    std::string line;

    while (std::getline(in, line)) {

        line = trim(line);

        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        if (line[0] == '[')
            continue;

        auto pos = line.find('=');

        if (pos == std::string::npos)
            continue;

        std::string key =
            trim(line.substr(0, pos));

        std::string value =
            trim(line.substr(pos + 1));

        values[key] = value;
    }

    return true;
}

std::string Config::getString(
    const std::string& key,
    const std::string& fallback) {

    if (values.count(key))
        return values[key];

    return fallback;
}

int Config::getInt(
    const std::string& key,
    int fallback) {

    if (!values.count(key))
        return fallback;

    try {
        return std::stoi(values[key]);
    }
    catch (...) {
        return fallback;
    }
}
