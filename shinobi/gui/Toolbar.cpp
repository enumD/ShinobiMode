#include "Toolbar.h"

void Toolbar::AddIcon(const std::string& id, const std::string& icon, bool visible, std::function<void()> onClick) {
    icons.push_back({id, icon, visible, onClick});
}

void Toolbar::ShowIcon(const std::string& id, bool show) {
    for (auto& icon : icons) {
        if (icon.id == id) {
            icon.visible = show;
            break;
        }
    }
}

// void Toolbar::Draw() {
//     ImGui::Begin("##Toolbar", nullptr, 
//         ImGuiWindowFlags_NoTitleBar | 
//         ImGuiWindowFlags_NoResize | 
//         ImGuiWindowFlags_NoMove | 
//         ImGuiWindowFlags_NoScrollbar
//     );

//     ImGui::SetWindowPos(ImVec2(0, 0));
//     ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 40));

//     const float padding = 20.0f;
//     float cursorX = padding;

//     for (const auto& icon : icons) {
//         if (!icon.visible) continue;

//         ImGui::SetCursorPosX(cursorX);
//         if (ImGui::Button(icon.icon.c_str())) {
//             if (icon.onClick) icon.onClick();
//         }
//         cursorX += ImGui::GetItemRectSize().x + padding;
//     }

//     ImGui::End();
// }

void Toolbar::Draw() {
 
        // Fixed at top with manual sizing
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(
            ImGui::GetIO().DisplaySize.x,
            40 // Fixed height
        ));
        
        ImGui::Begin("##Toolbar", nullptr, 
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings);
        
        // Toolbar content here
        for (const auto& icon : icons) {
            if (icon.visible) {
                if (ImGui::Button(icon.icon.c_str())) {
                    if (icon.onClick) icon.onClick();
                }
                ImGui::SameLine();
            }
        }
        
        ImGui::End();
    
}
