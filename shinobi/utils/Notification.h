// Notification.h
#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <imgui.h>  // Add this include for ImVec4

class Notification {
public:
    enum class Level { INFO, SUCCESS, WARNING, ERROR };
    
    static void Show(const std::string& message, Level level = Level::INFO, float duration = 3.0f);
    static void Render();
    
private:
    struct NotificationData {
        std::string message;
        Level level;
        std::chrono::time_point<std::chrono::steady_clock> showTime;
        float duration;
    };
    
    static std::vector<NotificationData> s_notifications;
    
    static ImVec4 GetLevelColor(Level level);
    static void RenderSingleNotification(const NotificationData& notification, float alpha);
};