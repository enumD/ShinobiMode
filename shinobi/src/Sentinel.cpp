#include "Sentinel.h"

Sentinel::Sentinel() : m_pBot(MyBot::getInstance()), m_myMode(SentinelModes::QUITE) { m_myName = "Sentinel"; }

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

    if (m_pBot)
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

            switch (m_myMode)
            {
            case SentinelModes::QUITE:
                if (isActive == true)
                {
                    m_myMode = SentinelModes::COUNTING;
                    m_vCount.push_back(1);
                }
                break;
            case SentinelModes::COUNTING:
            {
                // Store 1 (active) or 0 (inactive)
                m_vCount.push_back(isActive ? 1 : 0);

                // When buffer is full, compute the moving average
                if (m_countTo != 0 && m_vCount.size() >= m_countTo)
                {
                    // Compute correct average
                    double sum = std::accumulate(m_vCount.begin(), m_vCount.end(), 0.0);
                    double average = sum / m_countTo;

                    // Clear buffer for next window
                    m_vCount.clear();


                    // Decision logic
                    if (average > 0.5)
                    {
                        m_myMode = SentinelModes::ALARM;
                    }
                    else
                    {
                        m_myMode = SentinelModes::QUITE;
                    }
                }
            }
            break;
            case SentinelModes::ALARM:
            { // Do something
                if (m_pBot && m_pBot->isRunning())
                {
                    _signalToTelegram();
                }
                else
                {
                    _autoMode();
                }
            }
            break;
            case SentinelModes::NUM_OF_SENTINEL_MODES:
                m_myMode = SentinelModes::QUITE;
                break;

            default:
                m_myMode = SentinelModes::QUITE;

                break;
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
    auto future = m_pBot->SendMessageAsync("Detect alarm: Choose an option:\nNo: Nothing to do\nDog: play dog\nAlarm: play Alarm\nStop: Stop action");

    std::optional<std::string> response;

    // i is seconds: defailt 30
    for (int i = 0; i < 60; ++i)
    {
        if (future.wait_for(std::chrono::seconds(1)) == std::future_status::ready)
        {
            response = future.get();
            break;
        }

        if (m_bRunning == false)
        {
            // Called stop thread, abort waiting to prevent to remain append
            std::cerr << "Waiting for response aborted from main thread. return." << std::endl;

            m_pBot->deleteWaitingReplyFromSender();

            return; // Exit
        }
        std::cerr << "Waiting for response... " << (i + 1) << "s" << std::endl;
    }

    // Check value
    if (response.has_value())
    {
        std::cerr << "User replied: " << response.value() << std::endl;
        performUserAction(response.value());
    }
    else
    {
        // No response: timeout
        m_pBot->deleteWaitingReplyFromSender();
        std::cerr << "Sentinel() - Telegram no response, auto mode" << std::endl;
    }
}


void Sentinel::_autoMode() {}


void Sentinel::performUserAction(const std::string &action)
{
    std::string lowered = action;
    std::transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);

    if (lowered.find("no") != std::string::npos)
    {
        std::cout << "[ACTION] Nothing to do." << std::endl;
    }
    else if (lowered.find("dog") != std::string::npos)
    {
        std::cout << "[ACTION] Playing dog sound." << std::endl;
    }
    else if (lowered.find("alarm") != std::string::npos)
    {
        std::cout << "[ACTION] Playing alarm." << std::endl;
    }
    else if (lowered.find("stop") != std::string::npos)
    {
        std::cout << "[ACTION] Stopping actions." << std::endl;
    }
    else
    {
        std::cout << "[WARNING] Unknown action: " << action << std::endl;
    }
}
