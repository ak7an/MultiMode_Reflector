#include "config.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

static std::unordered_map<std::string, int> ints;

void Config::load(const std::string& file) {

    std::ifstream in(file);
    std::string line;

    while (std::getline(in, line)) {

        if (line.empty() || line[0] == '[')
            continue;

        auto pos = line.find('=');

        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        int value = std::stoi(line.substr(pos + 1));

        ints[key] = value;
    }
}

int Config::getInt(const std::string& key) {

    if (ints.count(key))
        return ints[key];

    return 0;
}
