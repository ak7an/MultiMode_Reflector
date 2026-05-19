#pragma once

#include <string>

class Config {
public:
    void load(const std::string& file);
    int getInt(const std::string& key);
};
