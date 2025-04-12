#pragma once
#include "../interface/IStartStop.h"
#include "../src/SensorMng.h"
#include "../utils/Logger.h"
#include "../utils/Vlcplayer.h"
#include <memory>
#include <thread>

class Modes : public IStartStop
{

  public:
    explicit Modes();

    Modes(const Modes &) = delete;
    Modes &operator=(const Modes &) = delete;
    Modes(Modes &&) = delete;
    Modes &operator=(Modes &&) = delete;

    void start();

    void stop();

    bool isRunning();

    ~Modes();

  public:
    std::string m_myName = "No Name";
    std::atomic<bool> m_bRunning;
    std::shared_ptr<SensorMng> m_pSensorMng;
    std::shared_ptr<Vlcplayer> m_pVlc;

    std::string m_dog_audio_folder_path = "audio";

  private:
    virtual void _thread_func() = 0;

    std::thread m_thread;
};