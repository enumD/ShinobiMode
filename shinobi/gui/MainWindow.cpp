#include "MainWindow.h"
#include <imgui_internal.h>

void MainWindow::initialize() {
   
    applyCustomStyle();
    if (!m_camera.availableDevices().empty()) {
        m_camera.start(m_camera.availableDevices()[0]);
    }
}
void MainWindow::shutdown()
{

}

void MainWindow::render() {
    // Occupa tutto lo spazio sotto la toolbar
    ImGui::SetNextWindowPos(ImVec2(0, 40));
    ImGui::SetNextWindowSize(ImVec2(
        ImGui::GetIO().DisplaySize.x,
        ImGui::GetIO().DisplaySize.y - 40
    ));

    ImGui::Begin("MainContent", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    // Contenuto principale qui (es. camera)
    renderCameraPanel();

    ImGui::End();
}


// void MainWindow::render() {
//     m_camera.updateFrame();

//     // Main application window
//     ImGui::SetNextWindowPos(ImVec2(0, 0));
//     ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
//     ImGui::Begin("MainApp", nullptr, 
//         ImGuiWindowFlags_NoTitleBar | 
//         ImGuiWindowFlags_NoResize | 
//         ImGuiWindowFlags_NoMove |
//         ImGuiWindowFlags_NoCollapse |
//         ImGuiWindowFlags_MenuBar);


//     renderCameraPanel();
    
  
//     ImGui::End();
// }

void MainWindow::renderCameraPanel() {
    ImGui::BeginChild("CameraView", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
    
    if (m_camera.isRunning()) {
        ImVec2 avail = ImGui::GetContentRegionAvail();
        ImGui::Image(m_camera.textureID(), avail);
    } else {
        ImGui::Text("No active camera stream");
    }

    ImGui::EndChild();

    // Device controls
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 5));
    if (ImGui::Button("Refresh Devices")) {
        m_camera.refreshDevices();
    }
    
    ImGui::SameLine();
    if (ImGui::BeginCombo("##Devices", 
        m_camera.availableDevices().empty() ? 
        "No devices" : 
        m_camera.availableDevices()[m_camera.currentDevice()].c_str())) {
        
        for (size_t i = 0; i < m_camera.availableDevices().size(); ++i) {
            if (ImGui::Selectable(m_camera.availableDevices()[i].c_str(), i == m_camera.currentDevice())) {
                m_camera.start(m_camera.availableDevices()[i]);
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopStyleVar();
}

void MainWindow::applyCustomStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Modern dark style
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    
    // Rounding and spacing
    style.WindowRounding = 0.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.WindowPadding = ImVec2(5, 5);
    style.FramePadding = ImVec2(8, 4);
    
    // Custom button colors
    ImVec4 accentColor(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button] = accentColor;
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(
        accentColor.x + 0.1f, 
        accentColor.y + 0.1f, 
        accentColor.z + 0.1f, 
        accentColor.w);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(
        accentColor.x + 0.2f, 
        accentColor.y + 0.2f, 
        accentColor.z + 0.2f, 
        accentColor.w);
}