#pragma once
#include "../interface/IStartStop.h"
#include "../src/SensorMng.h"
#include "../utils/Logger.h"
#include "../utils/SensorData.h"
#include "../utils/Vlcplayer.h"
#include "../utils/myGlobals.h"
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <thread>

class Dog : public IStartStop
{

  public:
    explicit Dog();

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

    std::thread m_thread;
    std::atomic<bool> m_bRunning;
    std::shared_ptr<SensorMng> m_pSensorMng;
    std::shared_ptr<Vlcplayer> m_pVlc;
    std::string m_dog_audio_folder_path = "audio";
};