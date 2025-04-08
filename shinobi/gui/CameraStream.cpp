#include "CameraStream.h"
#include <filesystem>
#include <thread> // Aggiunto per std::this_thread.sleep

CameraStream::CameraStream() : m_textureID(0), m_isRunning(false), m_currentDev_id(0)
{
    refreshSystemDevice();
}

CameraStream::~CameraStream()
{
    stop();

    if (m_textureID)
    {
        glDeleteTextures(1, &m_textureID);
    }
}

/// @brief Return current device id running
/// @return 
int CameraStream::currentDevice()
{
    return m_currentDev_id;
}


/// @brief Refresh the list of system device attached:/dev/video*
void CameraStream::refreshSystemDevice()
{
    m_devices.clear();
    for (int i = 0; i < 4; ++i)
    {
        std::string path = "/dev/video" + std::to_string(i);
        if (std::filesystem::exists(path))
        {
            m_devices.push_back(path);
        }
    }
}


/// @brief Start camera by id
/// @param dev_id 
/// @return 
bool CameraStream::start(int dev_id)
{
    stop(); // Ferma qualsiasi istanza precedente
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    m_currentDev_id = dev_id;
    std::cout << "Tentativo di aprire: " << m_devices[dev_id] << std::endl;

    if (!m_cap.open(m_devices[dev_id], cv::CAP_V4L2))
    {
        std::cerr << "Errore: Impossibile aprire il dispositivo " << m_devices[dev_id] << std::endl;
        return false;
    }

    std::cout << "Dispositivo aperto con successo!" << std::endl;

    m_isRunning = true;
    glGenTextures(1, &m_textureID);
    return true;
}


void CameraStream::stop()
{
    if (m_isRunning)
    {
        // Rilascia prima la texture
        if (m_textureID)
        {
            glDeleteTextures(1, &m_textureID);
            m_textureID = 0;
        }

        // Assicuriamoci che GStreamer abbia chiuso la pipeline
        if (m_cap.isOpened())
        {
            m_cap.set(cv::CAP_PROP_MODE, cv::VideoCaptureAPIs::CAP_GSTREAMER);
            m_cap.release();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Aspetta che GStreamer completi la chiusura

        m_isRunning = false;
    }
}

void CameraStream::updateFrame()
{
    if (!m_isRunning)
        return;

    m_cap >> m_frame;
    if (!m_frame.empty())
    {
        cv::cvtColor(m_frame, m_frame, cv::COLOR_BGR2RGB);
        convertFrameToTexture();
    }
    else
    {
        std::cerr << "⚠️ Frame vuoto! Potrebbe esserci un problema con la webcam." << std::endl;
    }
}

void CameraStream::convertFrameToTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_frame.cols, m_frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, m_frame.data);
}