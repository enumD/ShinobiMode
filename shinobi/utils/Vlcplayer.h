#pragma once
#include "Logger.h"
#include "myGlobals.h"
#include <atomic>
#include <filesystem>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vlc/vlc.h>

class Vlcplayer
{
  public:
    ~Vlcplayer();

    Vlcplayer(const Vlcplayer &) = delete;
    Vlcplayer &operator=(const Vlcplayer &) = delete;
    Vlcplayer(Vlcplayer &&) = delete;
    Vlcplayer &operator=(Vlcplayer &&) = delete;

    void Init();
    void Terminate();
    static std::shared_ptr<Vlcplayer> getInstance();
    void playFile(const std::string &filePath);
    void playRandomFromPath(const std::string &folderPath);
    void pause();
    void stop();

    bool isPlaying() const;

  private:
    Vlcplayer();
    std::string _getRandomAudio(const std::string &folderPath);

    static std::shared_ptr<Vlcplayer> m_myInstance;
    static std::mutex m_lock;

    std::unique_ptr<libvlc_instance_t, decltype(&libvlc_release)> m_pVlcInstance;
    std::unique_ptr<libvlc_media_player_t, decltype(&libvlc_media_player_release)> m_pMediaPlayer;
};
