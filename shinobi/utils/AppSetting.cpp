#include "AppSetting.h"

AppSetting::AppSetting()
{
    this->m_wifi_id="No ID";
    this->m_wifi_pswd = "Password";
    this->m_volume=50;
} 

AppSetting::AppSetting(const std::string& id, const std::string& pswd, const int Volume) : m_wifi_id(id), m_wifi_pswd(pswd), m_volume(0) {}


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