#pragma once
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class ConfigManager {
public:
    void Save(const std::string& path, const json& data);
    json Load(const std::string& path);
};