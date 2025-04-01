#ifndef APPSETTING_H
#define APPSETTING_H

#include <string>

class AppSetting {
public:
    std::string m_wifi_id;
    std::string m_wifi_pswd;

    AppSetting();
    AppSetting(const std::string& id, const std::string& pswd);
};

#endif // APPSETTING_H