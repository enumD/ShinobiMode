#include "dog.h"

Dog::Dog() : m_bRunning(false), m_pSensorMng(SensorMng::getInstance()), m_pVlc(Vlcplayer::getInstance()) {}

Dog::~Dog() { this->stop(); }

void Dog::stop()
{
    try
    {
        m_bRunning = false;

        m_pVlc->Terminate();

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
        m_pVlc->Init();

        m_bRunning = true;

        // Start thread
        m_thread = std::thread(&Dog::_thread_func, this);

        std::cout << "Dog::start() - thread started\n" << std::endl;
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

            bool bIspresent = std::any_of(tmpVector.begin(), tmpVector.end(), [](const auto &s) { return s.m_status != 0; });

            if (bIspresent == true)
            {
                m_pVlc->playRandomFromPath(this->m_dog_audio_folder_path);
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
