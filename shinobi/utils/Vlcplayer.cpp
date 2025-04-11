#include "Vlcplayer.h"

// S tatic members
std::shared_ptr<Vlcplayer> Vlcplayer::m_myInstance = nullptr;
std::mutex Vlcplayer::m_lock;

Vlcplayer::Vlcplayer() : m_pVlcInstance(nullptr, &libvlc_release), m_pMediaPlayer(nullptr, &libvlc_media_player_release) {}

Vlcplayer::~Vlcplayer() { Terminate(); }

void Vlcplayer::Init()
{
    libvlc_instance_t *rawInstance = libvlc_new(0, nullptr);
    if (rawInstance != nullptr)
    {
        m_pVlcInstance.reset(rawInstance);
    }
    else
    {
        Logger::log("Vlcplayer::Init() - Error init libVLC", true);
        throw std::runtime_error("Vlcplayer::Init() - nullptr vlc instance");
    }
}

void Vlcplayer::Terminate()
{
    this->stop();

    if (m_pVlcInstance)
    {
        m_pVlcInstance.reset();
    }
}

void Vlcplayer::stop()
{
    try
    {
        if (m_pMediaPlayer)
        {
            if (libvlc_media_player_is_playing(m_pMediaPlayer.get()))
            {
                libvlc_media_player_stop(m_pMediaPlayer.get());
            }

            m_pMediaPlayer.reset();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        Logger::log(e.what(), true);
    }
}

std::shared_ptr<Vlcplayer> Vlcplayer::getInstance()
{
    std::lock_guard<std::mutex> lock(m_lock);
    if (!m_myInstance)
    {
        m_myInstance.reset(new Vlcplayer());
    }
    return m_myInstance;
}

void Vlcplayer::playFile(const std::string &filePath)
{
    try
    {
        if (m_pMediaPlayer && libvlc_media_player_is_playing(m_pMediaPlayer.get()))
        {
            // Vlc already playing a sound nothing to do
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        else
        {
            // Only one thread at time can play
            std::lock_guard<std::mutex> lock(m_lock);

            // Stop possible previously played audio
            this->stop();

            libvlc_media_t *media = libvlc_media_new_path(m_pVlcInstance.get(), filePath.c_str());

            if (!media)
            {
                std::string logS = "Vlcplayer::playFile() - Cannot load file " + filePath;
                std::cerr << logS << std::endl;
                Logger::log(logS, true);
                return;
            }

            libvlc_media_player_t *rawPlayer = libvlc_media_player_new_from_media(media);

            libvlc_media_release(media); // player manage media from now on so release

            if (rawPlayer == nullptr)
            {
                std::cerr << "Vlcplayer::playFile() - Cannot create mediaplayer with media" << filePath.c_str() << std::endl;
                Logger::log("Vlcplayer::playFile() - Cannot create mediaplayer with media " + filePath, true);

                return;
            }

            m_pMediaPlayer.reset(rawPlayer);

            if (m_pMediaPlayer != nullptr)
            {
                libvlc_media_player_play(m_pMediaPlayer.get());
            }
            else
            {
                std::cerr << "Vlcplayer::playFile - Cannot reset mediaplayer " << filePath.c_str() << std::endl;
                Logger::log("Vlcplayer::playFile - Cannot reset mediaplayer ", true);

                return;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        Logger::log(e.what(), true);
    }
}

void Vlcplayer::playRandomFromPath(const std::string &folderPath)
{
    try
    {
        std::string randAudio = _getRandomAudio(folderPath);

        this->playFile(randAudio);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        Logger::log(e.what(), true);
    }
}

void Vlcplayer::pause() {}

bool Vlcplayer::isPlaying() const
{
    bool res = false;

    if (m_pMediaPlayer && libvlc_media_player_is_playing(m_pMediaPlayer.get()))
    {
        res = true;
    }
    return res;
}

std::string Vlcplayer::_getRandomAudio(const std::string &folderPath)
{
    std::vector<std::string> fileAudio;

    // Found audio file in the directory
    for (const auto &entry : std::filesystem::directory_iterator(folderPath))
    {
        std::string estensione = entry.path().extension().string();
        if (estensione == ".wav" || estensione == ".mp3")
        {
            fileAudio.push_back(entry.path().string());
        }
    }

    if (fileAudio.empty())
    {
        std::cerr << "Vlcplayer::_getRandomAudio - audio file" << std::endl;
        Logger::log("Vlcplayer::_getRandomAudio - No Audio file");
        throw std::runtime_error("No audio files found in folder: " + folderPath);
    }

    // Choose a random audio
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, fileAudio.size() - 1);
    std::string fileSelezionato = fileAudio[distrib(gen)];

    return fileSelezionato;
}