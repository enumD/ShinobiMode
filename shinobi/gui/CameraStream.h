#ifndef CAMERA_STREAM_H
#define CAMERA_STREAM_H

#include "imgui.h"
#include <GL/gl.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class CameraStream
{
  public:
    CameraStream();
    ~CameraStream();

    bool start(int dev_id);

    void stop();

    void refreshSystemDevice();

    void updateFrame();

    int currentDevice();

    float getAspectRatio() const
    {
        if (m_frame.empty())
            return 16.0f / 9.0f;
        return static_cast<float>(m_frame.cols) / m_frame.rows;
    }

    bool isRunning() const { return m_isRunning; }

    ImTextureID textureID() const { return (ImTextureID)(intptr_t)m_textureID; }

    const std::vector<std::string> &availableDevices() const { return m_devices; }

  private:
    cv::VideoCapture m_cap;
    cv::Mat m_frame;
    GLuint m_textureID = 0;
    bool m_isRunning = false;
    std::vector<std::string> m_devices;
    int m_currentDev_id;

    void convertFrameToTexture();
};

#endif
