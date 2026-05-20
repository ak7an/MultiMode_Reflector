#pragma once

#include <string>
#include <unordered_map>

class ConfigManager {
public:

    static bool load(
        const std::string& path);

    static std::string getString(
        const std::string& key);

    static int getInt(
        const std::string& key);

private:

    static std::unordered_map<
        std::string,
        std::string> m_config;
};
