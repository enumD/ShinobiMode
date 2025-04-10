#include "dog.h"

Dog::Dog()
    : m_bRunning(false), m_pSensorMng(nullptr), m_pVlcInstance(nullptr, libvlc_release),
      m_pMediaPlayer(nullptr, libvlc_media_player_release)
{
    m_pSensorMng = SensorMng::getInstance();
}

Dog::~Dog() { this->stop(); }

void Dog::stop()
{
    try
    {
        m_bRunning = false;

        this->_stopMediaPlayer();

        m_pVlcInstance.reset();

        if (m_thread.joinable())
        {

            m_thread.join();

            std::cout << "Dog::stop() - thread stopped \n" << std::endl;
        }
        else
        {
            std::cout << "Dog::stop() - thread was not joinable \n" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        Logger::log(e.what(), true);
        std::cout << e.what() << std::endl;
    }
}

bool Dog::isRunning() { return m_bRunning; }

/// @brief Start Thread.
/// @throws std::runtime_error if m_pSensormng is nullptr
void Dog::start()
{
    // Check sensor manager to prevent nulltpr on thread loop
    if (m_pSensorMng == nullptr)
    {
        Logger::log("Dog::start() - sensormng nullptr", true);
        throw std::runtime_error("Dog::start() - SensorMng nullptr");
    }

    if (m_bRunning == false)
    {
        _vlcInit();

        m_bRunning = true;

        // Start thread
        m_thread = std::thread(&Dog::_thread_func, this);

        std::cout << "Dog::start() - thread started\n" << std::endl;
    }
}

void Dog::_stopMediaPlayer()
{
    if (m_pMediaPlayer)
    {
        libvlc_media_player_stop(m_pMediaPlayer.get());
        m_pMediaPlayer.reset();
    }
}

void Dog::_vlcInit()
{
    libvlc_instance_t *rawInstance = libvlc_new(0, nullptr);
    if (rawInstance != nullptr)
    {
        m_pVlcInstance.reset(rawInstance);
    }
    else
    {
        Logger::log("Dog::_vlcInit() - Error init libVLC", true);
        throw std::runtime_error("nullptr vlc instance");
    }
}

void Dog::_thread_func()
{
    while (m_bRunning == true)
    {
        // DEBUG std::cout << "Dog thread do a cycle\n" << std::endl;

        try
        {
            // Get sensor state
            std::vector<SensorData> tmpVector = m_pSensorMng->getCurrentData();

            bool bIspresent =
                std::any_of(tmpVector.begin(), tmpVector.end(), [](const auto &s) { return s.m_status != 0; });

            if (bIspresent == true)
            {
                this->_playRandomBark();
            }
        }
        catch (const std::exception &e)
        {
            Logger::log(e.what());
            std::cerr << e.what() << '\n';
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_SLEEP_MILLI));
    }

    std::cout << "Dog::_thread_func() - SensorMng thread exited while loop!\n" << std::endl;
}

void Dog::_playRandomBark()
{
    if (m_pMediaPlayer != nullptr && libvlc_media_player_is_playing(m_pMediaPlayer.get()))
    {
        // Vlc already playing a sound nothing to do
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_SLEEP_MILLI));
    }
    else
    {
        // Stop previously media terminated
        _stopMediaPlayer();

        std::string randAudio = _getRandomAudio();

        libvlc_media_t *media = libvlc_media_new_path(m_pVlcInstance.get(), randAudio.c_str());
        if (!media)
        {
            std::cerr << "Dog::_playRandomBark() - Cannot load file: " << randAudio.c_str() << std::endl;
            Logger::log("Dog::_playRandomBark() - Cannot loadfile: ", true);

            return;
        }

        libvlc_media_player_t *rawPlayer = libvlc_media_player_new_from_media(media);

        libvlc_media_release(media); // player manage media nofrom now on so release

        if (rawPlayer == nullptr)
        {
            std::cerr << "Dog::_playRandomBark() - Cannot create mediaplayer with media" << randAudio.c_str()
                      << std::endl;
            Logger::log(
                std::string("Dog::_playRandomBark() - Cannot create mediaplayer with media ", randAudio.c_str()),
                true);

            return;
        }

        m_pMediaPlayer.reset(rawPlayer);

        if (m_pMediaPlayer != nullptr)
        {
            libvlc_media_player_play(m_pMediaPlayer.get());
        }
        else
        {
            std::cerr << "Dog::_playRandomBark() - Cannot reset mediaplayer " << randAudio.c_str() << std::endl;
            Logger::log("Dog::_playRandomBark() - Cannot reset mediaplayer ", true);

            return;
        }
    }
}

std::string Dog::_getRandomAudio()
{
    std::vector<std::string> fileAudio;

    // Found audio file in the directory
    for (const auto &entry : std::filesystem::directory_iterator("audio"))
    {
        std::string estensione = entry.path().extension().string();
        if (estensione == ".wav" || estensione == ".mp3")
        {
            fileAudio.push_back(entry.path().string());
        }
    }

    if (fileAudio.empty())
    {
        std::cerr << "No audio file" << std::endl;
        Logger::log("Dog::_getRandomAudio() - No Audio file");
        return "no audio file found in audio folder";
    }

    // Choose a random audio
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, fileAudio.size() - 1);
    std::string fileSelezionato = fileAudio[distrib(gen)];

    return fileSelezionato;
}
