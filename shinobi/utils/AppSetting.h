#ifndef APPSETTING_H
#define APPSETTING_H

#include <string>


class AppSetting {
public:
    std::string m_wifi_id;
    std::string m_wifi_pswd;
    int m_volume;

    AppSetting();
    AppSetting(const std::string& id, const std::string& pswd, const int Volume);
};

#endif // APPSETTING_H