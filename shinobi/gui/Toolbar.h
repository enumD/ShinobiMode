#pragma once
#include "imgui.h"
#include <functional>
#include <string>
#include <vector>

struct Icon
{
    std::string id;
    std::string icon;
    bool visible;
    std::function<void()> onClick;
    ImVec4 bgColor;
    ImVec4 textColor;
};

class Toolbar
{
  public:
    void Init();
    void AddIcon(const std::string &id, const std::string &icon, bool visible = true, std::function<void()> onClick = nullptr);
    void ShowIcon(const std::string &id, bool show);
    void Draw();

    void SetIconBgColor(const std::string &id, const ImVec4 &color);
    void SetIconText(const std::string &id, const std::string &text);
    void SetIconTextColor(const std::string &id, const ImVec4 &color);

  private:
    std::vector<Icon> icons;
};