#include "ConfigManager.h"
#include <fstream>

void ConfigManager::Save(const std::string& path, const json& data) {
    std::ofstream file(path);
    if (file.is_open()) {
        file << data.dump(4);
    }
}

json ConfigManager::Load(const std::string& path) {
    std::ifstream file(path);
    if (file.good()) {
        try {
            return json::parse(file);
        } catch (...) {
            return json();
        }
    }
    return json();
}