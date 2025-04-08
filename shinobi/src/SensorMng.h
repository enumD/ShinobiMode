#pragma once

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

#include "../utils/myGlobals.h"
#include "../utils/SensorData.h"
#include "../utils/Logger.h"


/// @brief This class poll sensors and collect their states. getCurrentData() return the most recent status
class SensorMng
{
public:
    SensorMng();

    ~SensorMng();

    void start();

    void stop();

    std::vector<SensorData> getCurrentData();

private:
    void sensorReaderThread();

    void thread_func();

    uint64_t getCurrentTimestamp();

private:
    std::vector<SensorData> m_sensors;

    std::thread m_thread;

    std::atomic<bool> m_bRunning;

    std::mutex m_lock;
};