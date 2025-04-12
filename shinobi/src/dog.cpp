#include "dog.h"

Dog::Dog() { m_myName = "Dog"; }

Dog::~Dog() {}

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

    std::cout << "Dog::_thread_func() - Dog thread exited while loop!\n" << std::endl;
}
