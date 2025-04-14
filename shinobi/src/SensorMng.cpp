
#include "SensorMng.h"

std::shared_ptr<SensorMng> SensorMng::m_instance = nullptr;

SensorMng::SensorMng() : m_bRunning(false)
{
    for (size_t i = 0; i < NUM_OF_SENSORS; i++)
    {
        m_sensors.push_back(SensorData((double)i, 0));
    }
}

SensorMng::~SensorMng()
{
    stop();

    if (m_sensors.size() > 0)
    {
        m_sensors.clear();
    }

    if (m_instance)
    {
        m_instance.reset();
    }
}

std::shared_ptr<SensorMng> SensorMng::getInstance()
{
    // Create a new SensorMng only is not exist yet
    if (!m_instance)
    {
        m_instance = std::shared_ptr<SensorMng>(new SensorMng());
    }
    return m_instance;
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
    catch (const std::exception &e)
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
    // Acquire lock
    std::lock_guard<std::mutex> guard(m_lock);

    std::vector<SensorData> tmpVector;

    for (auto i = 0; i < m_sensors.size(); i++)
    {
        tmpVector.push_back(SensorData(m_sensors[i].m_id, m_sensors[i].m_status));
    }

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
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            Logger::log(e.what());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_SLEEP_MILLI_SENSOR_MNG));
    }

    std::cout << "SensorMng thread exited while loop!\n" << std::endl;
}

void SensorMng::thread_func()
{
    std::lock_guard<std::mutex> guard(m_lock);

    for (auto &sensor : m_sensors)
    {
        // Alterna lo stato del sensore
        sensor.m_status = (sensor.m_status == 0) ? 1 : 0;
    }
}

uint64_t SensorMng::getCurrentTimestamp() { return std::chrono::system_clock::now().time_since_epoch().count(); }