#include "CameraWindow.h"
#include <imgui_internal.h>

void CameraWindow::initialize() {
   
    applyCustomStyle();
    m_camera.start(0);
 
}
void CameraWindow::shutdown()
{
    m_camera.stop();
}


void CameraWindow::render() 
{
    // Occupy all space available
    ImGui::BeginChild("CameraView", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
    
    if (m_camera.isRunning()) 
    {
        setCamera();
    } else {
        setVoidCamera();
    }
    
    // Set Cursor Pos for switch button
    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x - 120, 10));
    
    // Button Style
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.15f, 0.15f, 0.9f));

    // Create  button on the stream view
    if (ImGui::Button("Switch", ImVec2(100, 50))) {
        auto devices = m_camera.availableDevices();
        if (!devices.empty()) {
            int nextDevice = (m_camera.currentDevice() + 1) % devices.size();
            std::cerr << "\n dev " << nextDevice << std::endl;
            m_camera.start(nextDevice);
        }
    }

    // Reset predefined style
    ImGui::PopStyleColor(3);

    // Close Child
    ImGui::EndChild();
}


void CameraWindow::setCamera()
{
    m_camera.updateFrame();
        
    // Get  available dimension
    ImVec2 avail = ImGui::GetContentRegionAvail();
    
    // Calculate Aspect ration current frame
    float aspect = m_camera.getAspectRatio();
    
    // Calculate dimension by camera aspect ratio
    float targetWidth = avail.x;
    float targetHeight = targetWidth / aspect;
    
    // If too high scale
    if (targetHeight > avail.y) {
        targetHeight = avail.y;
        targetWidth = targetHeight * aspect;
    }
    
    // Calculate offset to center 
    float offsetX = (avail.x - targetWidth) * 0.5f;
    float offsetY = (avail.y - targetHeight) * 0.5f;
    
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
    
    // Show image
    ImGui::Image(m_camera.textureID(), 
               ImVec2(targetWidth, targetHeight),
               ImVec2(0,0), ImVec2(1,1),
               ImVec4(1,1,1,1), ImVec4(1,1,1,0.5f));
}

void CameraWindow::setVoidCamera()
{
     // Messaggio centrato
     ImVec2 centerPos = ImVec2(
        (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Camera non disponibile").x) * 0.5f,
        (ImGui::GetContentRegionAvail().y - 30) * 0.5f
    );
    
    // Sfondo semitrasparente per il messaggio
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(centerPos.x - 10, centerPos.y - 10),
        ImVec2(centerPos.x + ImGui::CalcTextSize("Camera non disponibile").x + 10, centerPos.y + 40),
        IM_COL32(20, 20, 20, 200),
        5.0f
    );
    
    ImGui::SetCursorPos(centerPos);
    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Camera non disponibile");
    
  
}

void CameraWindow::applyCustomStyle()
{
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