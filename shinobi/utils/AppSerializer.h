// AppSerializer.h
#ifndef APPSERIALIZER_H
#define APPSERIALIZER_H

#include "AppSetting.h"
#include <string>

class AppSerializer {
public:
    static void serialize(const AppSetting& settings, const std::string& filename);
    static AppSetting deserialize(const std::string& filename);
};

#endif // APPSERIALIZER_H
