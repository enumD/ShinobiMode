#pragma once

#include <imgui.h>
#include <string>
#include <vector>
#include <functional>

class ModeSelector {
public:
   
    
    // Render the ImGui interface
    void render();
    void init();
    void shutdown();

    // Get currently selected mode index
    int GetSelectedMode() const;
    
    // Get name of currently selected mode
    std::string GetSelectedModeName() const;

private:
    int selectedMode; // Index of selected mode
    std::vector<std::pair<std::string, bool>> modes; // List of modes (name, state)
};

