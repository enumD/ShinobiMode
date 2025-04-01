#pragma once
#include <vector>
#include <string>
#include <functional>
#include "imgui.h"

struct Icon {
    std::string id;
    std::string icon;
    bool visible;
    std::function<void()> onClick;
};

class Toolbar {
public:
    void Init();
    void AddIcon(const std::string& id, const std::string& icon, bool visible = true, std::function<void()> onClick = nullptr);
    void ShowIcon(const std::string& id, bool show);
    void Draw();

private:
   
    std::vector<Icon> icons;
};