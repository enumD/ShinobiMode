
#include "SensorMng.h"

SensorMng::SensorMng() : m_bRunning(false)
{

    for (size_t i = 0; i < NUM_OF_SENSORS; i++)
    {
        m_sensors.push_back(SensorData((double)i, 0));
    }
}

SensorMng::~SensorMng()
{
    if (m_thread.joinable())
    {
        stop();
    }
}

void SensorMng::stop()
{
    try
    {
        m_bRunning = false;

        if (m_thread.joinable())
        {
            m_thread.join();

            std::cout << "SensorMng stopped \n" << std::endl;
        }
        else
        {
            std::cout << "SensorMng was not joinable \n" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        Logger::log(e.what(), true);
    }
}

void SensorMng::start()
{
    if (m_bRunning == false)
    {
        m_bRunning = true;

        // Start thread
        m_thread = std::thread(&SensorMng::sensorReaderThread, this);

        std::cout << "SensorMng started\n" << std::endl;
    }
}

/// @brief Method used to get sensor data
/// @return Last SensorData Available
std::vector<SensorData> SensorMng::getCurrentData()
{
    std::vector<SensorData> tmpVector;

    // Acquire lock
    m_lock.lock();

    for (auto i = 0; i < m_sensors.size(); i++)
    {
        tmpVector.push_back(SensorData(m_sensors[i].m_id, m_sensors[i].m_status));
    }

    m_lock.unlock();

    return tmpVector;
}

void SensorMng::sensorReaderThread()
{
    while (m_bRunning == true)
    {
        // DEBUG std::cout << "SensorMng thread do a cycle\n" << std::endl;

        try
        {
            thread_func();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            Logger::log(e.what());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_SLEEP_MILLI));
    }

    std::cout << "SensorMng thread exited while loop!\n" << std::endl;
}

void SensorMng::thread_func()
{
    m_lock.lock();

    for (auto i = 0; i < m_sensors.size(); i++)
    {
        if (m_sensors[i].m_status == 0)
        {
            m_sensors[i].m_status = 1;
        }
        else
        {
            m_sensors[i].m_status = 0;
        }
    }

    m_lock.unlock();
}

uint64_t SensorMng::getCurrentTimestamp()
{
    return std::chrono::system_clock::now().time_since_epoch().count();
}