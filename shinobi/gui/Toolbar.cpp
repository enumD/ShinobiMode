#include "Toolbar.h"

void Toolbar::AddIcon(const std::string &id, const std::string &icon, bool visible, std::function<void()> onClick) { icons.push_back({id, icon, visible, onClick, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)}); }

void Toolbar::ShowIcon(const std::string &id, bool show)
{
    for (auto &icon : icons)
    {
        if (icon.id == id)
        {
            icon.visible = show;
            break;
        }
    }
}


void Toolbar::Draw()
{

    // Fixed at top with manual sizing
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x,
                                    30 // Fixed height
                                    ));

    ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings);

    // Set trasparent background for buttons
    // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

    // Toolbar content here
    for (const auto &icon : icons)
    {
        if (icon.visible)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, icon.textColor);
            ImGui::PushStyleColor(ImGuiCol_Button, icon.bgColor);

            if (ImGui::Button(icon.icon.c_str()))
            {
                if (icon.onClick)
                    icon.onClick();
            }


            ImGui::SameLine();
            ImGui::PopStyleColor(1);
            ImGui::PopStyleColor(1);
        }
    }

    // ImGui::PopStyleColor(1);

    ImGui::End();
}


void Toolbar::SetIconBgColor(const std::string &id, const ImVec4 &color)
{
    for (auto &icon : icons)
    {
        if (icon.id == id)
        {
            icon.bgColor = color;
            break;
        }
    }
}

void Toolbar::SetIconText(const std::string &id, const std::string &text)
{
    for (auto &icon : icons)
    {
        if (icon.id == id)
        {
            icon.icon = text;
            break;
        }
    }
}

void Toolbar::SetIconTextColor(const std::string &id, const ImVec4 &color)
{
    for (auto &icon : icons)
    {
        if (icon.id == id)
        {
            icon.textColor = color;
            break;
        }
    }
}