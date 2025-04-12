#include "Modes.h"

Modes::Modes() : m_bRunning(false), m_pSensorMng(SensorMng::getInstance()), m_pVlc(Vlcplayer::getInstance()) {}

Modes::~Modes() { this->stop(); }

void Modes::start()
{ // Check sensor manager to prevent nulltpr on thread loop
    if (m_pSensorMng == nullptr)
    {
        Logger::log(m_myName + "::start() - sensormng nullptr", true);
        throw std::runtime_error(m_myName + "::start() - SensorMng nullptr");
    }

    if (m_bRunning == false)
    {
        m_pVlc->Init();

        m_bRunning = true;

        // Start thread
        m_thread = std::thread(&Modes::_thread_func, this);

        std::cout << m_myName << "::start() - thread started\n" << std::endl;
    }
}

void Modes::stop()
{
    try
    {
        m_bRunning = false;

        if (m_thread.joinable())
        {
            m_thread.join();

            m_pVlc->Terminate();

            std::cout << m_myName << "::stop() - thread stopped \n" << std::endl;
        }
        else
        {
            std::cout << m_myName << "::stop() - thread was not joinable \n" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        Logger::log(e.what(), true);
        std::cout << e.what() << std::endl;
    }
}

bool Modes::isRunning() { return m_bRunning; }
