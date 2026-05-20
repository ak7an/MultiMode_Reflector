#include "config_manager.h"

#include "logger.h"

#include <fstream>
#include <sstream>

std::unordered_map<
    std::string,
    std::string>
ConfigManager::m_config;

bool ConfigManager::load(
    const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open()) {

        Logger::log(ERROR,
            "Failed to open config: " +
            path);

        return false;
    }

    std::string line;

    while (std::getline(file, line)) {

        if (line.empty())
            continue;

        if (line[0] == '[')
            continue;

        size_t eq =
            line.find('=');

        if (eq == std::string::npos)
            continue;

        std::string key =
            line.substr(0, eq);

        std::string value =
            line.substr(eq + 1);

        m_config[key] = value;
    }

    Logger::log(INFO,
        "Configuration loaded");

    return true;
}

std::string ConfigManager::getString(
    const std::string& key)
{
    return m_config[key];
}

int ConfigManager::getInt(
    const std::string& key)
{
    return std::stoi(
        m_config[key]);
}
