#ifndef CAMERA_WINDOW_H
#define CAMERA_WINDOW_H

#include "CameraStream.h"

class CameraWindow
{
  public:
    void initialize();
    void render();
    void shutdown();
    CameraStream &camera() { return m_camera; }

  private:
    CameraStream m_camera;

    bool m_showSettings = false;

    void setCamera();
    void setVoidCamera();


    void renderCameraPanel();
    void applyCustomStyle();
};

#endif