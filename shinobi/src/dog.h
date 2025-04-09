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
#include <mutex>
#include <random>
#include <thread>
#include <vlc/vlc.h>

class Dog : public IStartStop
{

  public:
    Dog(SensorMng *Sensormng);

    void start();

    void stop();

    bool isRunning();

    ~Dog();

    static void handleMediaEnd(const libvlc_event_t *event, void *data);

  private:
    void _thread_func();

    void _waitForAudioToFinish();

    void _removeMediaEndEvent();

    void _playRandomBark();

    void _createVlcMedia(const char *filepath);

    void _stopMediaPlayer();

    void _vlcInit();

    std::string _getRandomAudio();

    std::thread m_thread;

    std::atomic<bool> m_bRunning;

    SensorMng *m_pSensormng;
    libvlc_instance_t *m_pVlcInstance;
    libvlc_media_player_t *m_pMediaPlayer;

    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_audioFinished;
};