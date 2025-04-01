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

    bool start(const std::string& device);
    void stop();
    void refreshDevices();
    void updateFrame();
    int currentDevice();
    
    bool isRunning() const { return m_isRunning; }
    ImTextureID textureID() const { return (ImTextureID)(intptr_t)m_textureID; }
    const std::vector<std::string>& availableDevices() const { return m_devices; }

private:
    
    cv::VideoCapture m_cap;
    cv::Mat m_frame;
    GLuint m_textureID = 0;
    bool m_isRunning = false;
    std::vector<std::string> m_devices;
    
    void convertFrameToTexture();
};

