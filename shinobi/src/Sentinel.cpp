#include "Sentinel.h"

Sentinel::Sentinel() : m_pBot(MyBot::getInstance()) { m_myName = "Sentinel"; }

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
        m_pBot->stop();
    }
}

void Sentinel::_thread_func()
{

    while (m_bRunning == true)
    {
        // DEBUG std::cout << "Dog thread do a cycle\n" << std::endl;

        try
        {
            // Get sensor data (consider adding a mutex if shared)
            const auto sensorData = m_pSensorMng->getCurrentData();

            // Check if any sensor is active (status != 0)
            const bool isActive = std::any_of(sensorData.begin(), sensorData.end(), [](const SensorData &s) { return s.m_status != 0; });

            // Store 1 (active) or 0 (inactive)
            m_vCount.push_back(isActive ? 1 : 0);

            // When buffer is full, compute the moving average
            if (m_countTo != 0 && m_vCount.size() >= m_countTo)
            {
                // Compute correct average
                double sum = std::accumulate(m_vCount.begin(), m_vCount.end(), 0.0);
                double average = sum / m_countTo;

                // Clear buffer for next window (or use a circular buffer)
                m_vCount.clear();

                // Decision logic
                if (average > 0.5)
                {
                    // Do something
                    if (m_pBot && m_pBot->isRunning())
                    {
                        _signalToTelegram();
                    }
                    else
                    {
                        _autoMode();
                    }
                }
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

void Sentinel::_signalToTelegram()
{
    // auto future = m_pBot->SendMessageAsync("Figlio di puttana");

    // if (future.wait_for(std::chrono::seconds(20)) == std::future_status::timeout)
    // {
    //     std::cerr << "Timeout! La risposta non è arrivata in tempo" << std::endl;
    // }
    // else
    // {
    //     // Risposta arrivata in tempo
    //     auto respo = future.get();
    //     std::cout << "Risposta arrivata con msg ID: " << respo << std::endl;
    // }
}

void Sentinel::_autoMode() {}