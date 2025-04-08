#include "dog.h"

Dog::Dog() : m_bRunning(false)
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
    catch (const std::exception& e)
    {
        Logger::log(e.what(), true);
    }
}

void Dog::start()
{
    if (m_bRunning == false)
    {
        m_bRunning = true;

        // Start thread
        m_thread = std::thread(&Dog::thread_func, this);

        std::cout << "Dog thread started\n" << std::endl;
    }
}


void Dog::thread_func()
{
    while (m_bRunning == true)
    {
        // DEBUG std::cout << "Dog thread do a cycle\n" << std::endl;

        try
        {
            // Implement Dog logic
        }
        catch (const std::exception& e)
        {
            Logger::log(e.what());
            std::cerr << e.what() << '\n';
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_SLEEP_MILLI));
    }

    std::cout << "SensorMng thread exited while loop!\n" << std::endl;
}