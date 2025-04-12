#include "Sentinel.h"

Sentinel::Sentinel() : m_pBot(Bot::getInstance()) { m_myName = "Sentinel"; }

Sentinel::~Sentinel() {}

void Sentinel::start()
{
    if (m_pBot && m_pBot->isRunning() == false)
    {
        m_pBot->start();
    }

    Modes::start();
}

void Sentinel::stop()
{
    Modes::stop();

    if (m_pBot && m_pBot->isRunning())
    {
        m_pBot->shutdown();
    }
}

void Sentinel::_thread_func()
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

    std::cout << "Sentinel::_thread_func() - Sentinel thread exited while loop!\n" << std::endl;
}
