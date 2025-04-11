#pragma once
#include "../interface/IStartStop.h"
#include "../src/SensorMng.h"
#include "../utils/Logger.h"
#include "../utils/SensorData.h"
#include "../utils/Vlcplayer.h"
#include "../utils/myGlobals.h"

class Sentinel : public IStartStop
{

  public:
    explicit Sentinel();

    void start();

    void stop();

    bool isRunning();

    ~Sentinel();

    // Delete copy and move constructors and assignment operators: Used to prevent Dog to be copied
    // and moved, because it is not thread safe
    Sentinel(const Sentinel &) = delete;
    Sentinel &operator=(const Sentinel &) = delete;
    Sentinel(Sentinel &&) = delete;
    Sentinel &operator=(Sentinel &&) = delete;
    // End of

  private:
    void _thread_func();

    void _playRandomBark();

    void _stopMediaPlayer();

    void _vlcInit();

    std::string _getRandomAudio();

    std::thread m_thread;
    std::atomic<bool> m_bRunning;
    std::shared_ptr<SensorMng> m_pSensorMng;
};