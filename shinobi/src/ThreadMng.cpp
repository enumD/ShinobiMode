#include "ThreadMng.h"

ThreadMng::ThreadMng() : m_pSensorMng(SensorMng::getInstance()), m_dog(), m_sentinel()
{
    // Add modes to thread manager
    m_threads.emplace_back(std::make_pair(AlarmMode::DOG_MODE, std::ref(m_dog)));
    m_threads.emplace_back(std::make_pair(AlarmMode::SENTINEL_MODE, std::ref(m_sentinel)));
}

ThreadMng::~ThreadMng()
{
    Terminate();

    m_threads.clear();
}

void ThreadMng::Init()
{
    // Start Sensor Manager
    m_pSensorMng->start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

/// @brief Stop all thread
void ThreadMng::Terminate()
{
    for (auto i = 0; i < m_threads.size(); i++)
    {
        m_threads[i].second.stop();
    }

    m_pSensorMng->stop();
}

/// @brief Stop all threads and then start the passed one
/// @param Mode
void ThreadMng::SetThread(AlarmMode Mode)
{
    try
    {

        // Stop sensormng if no mode is selected
        // if (Mode == AlarmMode::NO_MODE)
        // {
        //     m_sensorMng.stop();
        // }

        auto istart = -1;
        for (auto i = 0; i < m_threads.size(); i++)
        {
            m_threads[i].second.stop();

            if (m_threads[i].first == Mode)
            {
                istart = i;
            }
        }

        if (istart >= 0 && istart < m_threads.size())
        {
            m_threads[istart].second.start();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        Logger::log(e.what());
    }
}
