#pragma once

#include <string>
#include <unordered_map>

class Config {
public:
    bool load(const std::string& file);

    std::string getString(
        const std::string& key,
        const std::string& fallback = "");

    int getInt(
        const std::string& key,
        int fallback = 0);

private:
    std::unordered_map<std::string, std::string> values;

    static std::string trim(const std::string& s);
};
