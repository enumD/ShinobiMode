#include "dog.h"

Dog::Dog(SensorMng *Sensormng)
    : m_bRunning(false), m_pSensormng(Sensormng), m_pVlcInstance(nullptr), m_pMediaPlayer(nullptr),
      m_audioFinished(false)
{
}

Dog::~Dog()
{
    if (this->m_thread.joinable())
    {
        this->stop();
    }
}

void Dog::stop()
{
    try
    {
        m_bRunning = false;

        this->_stopMediaPlayer();

        if (m_pVlcInstance)
        {
            libvlc_release(m_pVlcInstance);
            m_pVlcInstance = nullptr;
        }

        if (m_thread.joinable())
        {

            m_thread.join();

            std::cout << "Dog thread stopped \n" << std::endl;
        }
        else
        {
            std::cout << "Dog thread was not joinable \n" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        Logger::log(e.what(), true);
    }
}

/// @brief Start Thread.
/// @throws std::runtime_error if m_pSensormng is nullptr
void Dog::start()
{
    // Check sensor manager to prevent nulltpr on thread loop
    if (m_pSensormng == nullptr)
    {
        throw std::runtime_error("nullptr");
    }

    if (m_bRunning == false)
    {
        _vlcInit();

        m_bRunning = true;

        // Start thread
        m_thread = std::thread(&Dog::_thread_func, this);

        std::cout << "Dog thread started\n" << std::endl;
    }
}

void Dog::_stopMediaPlayer()
{
    if (m_pMediaPlayer)
    {
        libvlc_media_player_stop(m_pMediaPlayer);
        libvlc_media_player_release(m_pMediaPlayer);
        m_pMediaPlayer = nullptr;
    }
}

void Dog::_vlcInit()
{

    // Create Vlc instance used to play sound
    m_pVlcInstance = libvlc_new(0, nullptr);
    if (!m_pVlcInstance)
    {
        std::cerr << "Error init libVLC" << std::endl;
        throw std::runtime_error("nullptr vlc instance");
    }
}

void Dog::_createVlcMedia(const char *filepath)
{

    // 2. Crea un media
    libvlc_media_t *media = libvlc_media_new_path(m_pVlcInstance, filepath);
    if (!media)
    {
        std::cerr << "Cannot load file: " << filepath << std::endl;
        return;
    }

    // 3. Crea un media player
    libvlc_media_player_t *player = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media); // Il media ora è gestito dal media player

    if (!player)
    {
        std::cerr << "Cannot create media player" << std::endl;
        return;
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
            std::vector<SensorData> tmpVector = m_pSensormng->getCurrentData();

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

    std::cout << "SensorMng thread exited while loop!\n" << std::endl;
}

void Dog::_playRandomBark()
{
    _stopMediaPlayer();

    std::string randAudio = _getRandomAudio();

    libvlc_media_t *media = libvlc_media_new_path(m_pVlcInstance, randAudio.c_str());
    if (!media)
    {
        std::cerr << "Cannot load file: " << randAudio.c_str() << std::endl;
        return;
    }

    m_pMediaPlayer = libvlc_media_player_new_from_media(media);

    libvlc_media_release(media); // player manage media nofrom now on so release

    if (!m_pMediaPlayer)
    {
        std::cerr << "Cannot create mediaplayer with media" << randAudio.c_str() << std::endl;
        return;
    }

    libvlc_media_player_play(m_pMediaPlayer);

    _waitForAudioToFinish();

    _removeMediaEndEvent();

    _stopMediaPlayer();
}

std::string Dog::_getRandomAudio()
{
    std::vector<std::string> fileAudio;

    // Found audio file in the directory
    for (const auto &entry : std::filesystem::directory_iterator("/audio"))
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
        Logger::log("No Audio file");
        return "no audio file found in audio folder";
    }

    // Choose a random audio
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, fileAudio.size() - 1);
    std::string fileSelezionato = fileAudio[distrib(gen)];

    return fileSelezionato;
}

// Callback chiamato da VLC quando finisce il media
void Dog::handleMediaEnd(const libvlc_event_t *event, void *data)
{
    Dog *dog = static_cast<Dog *>(data);
    if (dog != nullptr)
    {
        std::unique_lock<std::mutex> lock(dog->m_mutex);
        dog->m_audioFinished = true;
        dog->m_cv.notify_one();
    }
}

void Dog::_waitForAudioToFinish()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [this]() { return m_audioFinished; });
}

void Dog::_removeMediaEndEvent()
{
    libvlc_event_manager_t *em = libvlc_media_player_event_manager(m_pMediaPlayer);
    libvlc_event_detach(em, libvlc_MediaPlayerEndReached, handleMediaEnd, this);
}
