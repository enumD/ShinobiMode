// Notification.cpp
#include "Notification.h"
#include <imgui.h>
#include <vector>
#include <algorithm>

std::vector<Notification::NotificationData> Notification::s_notifications;

void Notification::Show(const std::string& message, Level level, float duration) {
    s_notifications.push_back({
        message,
        level,
        std::chrono::steady_clock::now(),
        duration
    });
}

ImVec4 Notification::GetLevelColor(Level level) {
    switch(level) {
        case Level::SUCCESS: return ImVec4(0.0f, 0.7f, 0.0f, 1.0f);
        case Level::WARNING: return ImVec4(0.9f, 0.6f, 0.0f, 1.0f);
        case Level::ERROR:   return ImVec4(0.8f, 0.0f, 0.0f, 1.0f);
        default:            return ImVec4(0.1f, 0.5f, 0.8f, 1.0f); // INFO
    }
}

void Notification::RenderSingleNotification(const NotificationData& notification, float alpha) {
    const float padding = 10.0f;
    const float spacing = 5.0f;
    
    // Calculate position (top-right, stacked)
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 windowPos = ImVec2(io.DisplaySize.x - padding, padding);
    ImVec2 windowPivot = ImVec2(1.0f, 0.0f);
    
    // Set window style
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.7f * alpha));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));
    
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPivot);
    if (ImGui::Begin("##NotificationWindow", nullptr, 
        ImGuiWindowFlags_NoDecoration | 
        ImGuiWindowFlags_AlwaysAutoResize | 
        ImGuiWindowFlags_NoSavedSettings | 
        ImGuiWindowFlags_NoFocusOnAppearing | 
        ImGuiWindowFlags_NoNav)) {
        
        // Level indicator (left border)
        ImVec4 levelColor = GetLevelColor(notification.level);
        levelColor.w *= alpha; // Apply fade
        ImGui::PushStyleColor(ImGuiCol_ChildBg, levelColor);
        ImGui::BeginChild("##LevelIndicator", ImVec2(20, ImGui::GetTextLineHeight() + 4), false);
        ImGui::EndChild();
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        
        // Notification content
        ImGui::BeginGroup();
        {
            // Title based on level
            const char* title = "";
            switch(notification.level) {
                case Level::SUCCESS: title = "Success"; break;
                case Level::WARNING: title = "Warning"; break;
                case Level::ERROR:   title = "Error";   break;
                default:            title = "Info";     break;
            }
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, alpha));
            ImGui::TextUnformatted(title);
            ImGui::PopStyleColor();
            
            // Message text
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
            ImGui::TextUnformatted(notification.message.c_str());
            ImGui::PopTextWrapPos();
        }
        ImGui::EndGroup();
        
        ImGui::End();
    }
    
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();
}

void Notification::Render() {
    if (s_notifications.empty()) return;
    
    auto currentTime = std::chrono::steady_clock::now();
    
    // Remove expired notifications
    s_notifications.erase(
        std::remove_if(s_notifications.begin(), s_notifications.end(),
            [&currentTime](const NotificationData& n) {
                float elapsed = std::chrono::duration<float>(currentTime - n.showTime).count();
                return elapsed >= n.duration;
            }),
        s_notifications.end()
    );
    
    // Render all active notifications (newest on top)
    const float startY = 10.0f;
    float currentY = startY;
    
    for (auto& notification : s_notifications) {
        float elapsed = std::chrono::duration<float>(currentTime - notification.showTime).count();
        float remaining = notification.duration - elapsed;
        
        // Fade animation (last 0.5 seconds)
        float alpha = 1.0f;
        if (remaining < 0.5f) {
            alpha = remaining * 2.0f; // Linear fade
        }
        
        // Position calculation with vertical offset
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 10.0f, currentY), 
                              ImGuiCond_Always, ImVec2(1.0f, 0.0f));
        
        RenderSingleNotification(notification, alpha);
        
        // Update vertical position for next notification
        if (ImGui::IsWindowAppearing()) {
            currentY += ImGui::GetWindowHeight() + 5.0f;
        }
    }
}