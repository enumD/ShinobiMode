#include "CameraStream.h"
#include <filesystem>

CameraStream::CameraStream() {
    glGenTextures(1, &m_textureID);
    refreshDevices();
}

CameraStream::~CameraStream() {
    stop();
    glDeleteTextures(1, &m_textureID);
}

int CameraStream::currentDevice(){
    return 0;
}

void CameraStream::refreshDevices() {
    m_devices.clear();
    for (int i = 0; i < 4; ++i) {
        std::string path = "/dev/video" + std::to_string(i);
        if (std::filesystem::exists(path)) {
            m_devices.push_back(path);
        }
    }
}

bool CameraStream::start(const std::string& device) {
    stop();
    if (m_cap.open(device)) {
        m_isRunning = true;
        return true;
    }
    return false;
}

void CameraStream::stop() {
    if (m_cap.isOpened()) {
        m_cap.release();
    }
    m_isRunning = false;
}

void CameraStream::updateFrame() {
    if (!m_isRunning) return;
    
    m_cap >> m_frame;
    if (!m_frame.empty()) {
        cv::cvtColor(m_frame, m_frame, cv::COLOR_BGR2RGB);
        convertFrameToTexture();
    }
}

void CameraStream::convertFrameToTexture() {
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_frame.cols, m_frame.rows, 
                0, GL_RGB, GL_UNSIGNED_BYTE, m_frame.data);
}