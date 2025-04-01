#include "MessageBox.h"

void MessageBox::Show(const std::string& message) {
    ImGui::OpenPopup("Popup");
    
    if (ImGui::BeginPopupModal("Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("%s", message.c_str());
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
