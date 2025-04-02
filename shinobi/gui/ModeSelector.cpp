#include "ModeSelector.h"



void ModeSelector::init()
{
    selectedMode = 0;
// Initialize available modes
modes = {
    {"Babbione Mode", false},
    {"Sentinel Mode", false},
    {"Shinobi Mode", false},
    {"Dog Mode", false}
};

// Select first mode by default
if (!modes.empty()) {
    modes[0].second = true;
}

}

void ModeSelector::render() 
{
    ImGui::BeginChild("Mode Selector", ImVec2(0, 0), true);

    // Title
    ImGui::Text("Select operational mode:");
    ImGui::Separator();
 

    // Radio buttons for modes
    for (int i = 0; i < modes.size(); ++i) {
        bool isSelected = (selectedMode == i);
        if (ImGui::RadioButton(modes[i].first.c_str(), isSelected)) {
            selectedMode = i;
            // Update all modes' states
            for (auto& mode : modes) {
                mode.second = false;
            }
            modes[i].second = true;
        }

        // Show state (On/Off)
        ImGui::SameLine();
        ImGui::TextColored(modes[i].second ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), 
                         modes[i].second ? "On" : "Off");
    }

    ImGui::Separator();

    // Save button
    if (ImGui::Button("Save")) 
    {
       
    }

    ImGui::EndChild();
}



int ModeSelector::GetSelectedMode() const {
    return selectedMode;
}

std::string ModeSelector::GetSelectedModeName() const {
    if (selectedMode >= 0 && selectedMode < modes.size()) {
        return modes[selectedMode].first;
    }
    return "";
}

void ModeSelector::shutdown() 
{
    // Cleanup if needed
}