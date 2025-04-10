#pragma once
#include "../interface/IStartStop.h"
#include "../src/SensorMng.h"
#include "../utils/Logger.h"
#include "../utils/SensorData.h"
#include "../utils/myGlobals.h"
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <vlc/vlc.h>

class Dog : public IStartStop
{

  public:
    explicit Dog(SensorMng *Sensormng);

    void start();

    void stop();

    bool isRunning();

    ~Dog();

    // Delete copy and move constructors and assignment operators: Used to prevent Dog to be copied
    // and moved, because it is not thread safe
    Dog(const Dog &) = delete;
    Dog &operator=(const Dog &) = delete;
    Dog(Dog &&) = delete;
    Dog &operator=(Dog &&) = delete;
    // End of

  private:
    void _thread_func();

    void _playRandomBark();

    void _stopMediaPlayer();

    void _vlcInit();

    std::string _getRandomAudio();

    std::thread m_thread;
    std::atomic<bool> m_bRunning;
    SensorMng *m_pSensormng;
    std::unique_ptr<libvlc_instance_t, decltype(&libvlc_release)> m_pVlcInstance;
    std::unique_ptr<libvlc_media_player_t, decltype(&libvlc_media_player_release)> m_pMediaPlayer;
};