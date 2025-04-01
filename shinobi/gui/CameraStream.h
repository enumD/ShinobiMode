#pragma once

#include <opencv2/opencv.hpp>
#include <GL/gl.h>
#include <vector>
#include <string>
#include "imgui.h"

class CameraStream {
public:
    CameraStream();
    ~CameraStream();

    bool start(int dev_id);
    void stop();
    void refreshDevices();
    void updateFrame();
    int currentDevice();
    
    bool isRunning() const { return m_isRunning; }
    ImTextureID textureID() const { return (ImTextureID)(intptr_t)m_textureID; }
    const std::vector<std::string>& availableDevices() const { return m_devices; }
    float getAspectRatio() const {
        if (m_frame.empty()) return 16.0f/9.0f; // Default 16:9
        return static_cast<float>(m_frame.cols) / m_frame.rows;
    }
private:
    
    cv::VideoCapture m_cap;
    cv::Mat m_frame;
    GLuint m_textureID = 0;
    bool m_isRunning = false;
    std::vector<std::string> m_devices;
    int m_currentDev_id;
    
    void convertFrameToTexture();
};

