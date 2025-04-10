#pragma once

#include "../utils/Logger.h"
#include "../utils/SensorData.h"
#include "../utils/myGlobals.h"
#include <atomic>
#include <memory> // Per shared_ptr
#include <mutex>
#include <thread>
#include <vector>

/// @brief This class poll sensors and collect their states. getCurrentData() return the most recent status
class SensorMng
{
  public:
    /// @brief Method used to get the singleton instance of SensorMng
    /// @return SensorMng instance
    /// @throws std::runtime_error if instance is nullptr
    static std::shared_ptr<SensorMng> getInstance();

    void start();

    void stop();

    ~SensorMng();

    std::vector<SensorData> getCurrentData();

    // Disable copy and moving cause is a singleton
    SensorMng(const SensorMng &) = delete;
    SensorMng &operator=(const SensorMng &) = delete;
    SensorMng(SensorMng &&) = delete;
    SensorMng &operator=(SensorMng &&) = delete;

  private:
    void sensorReaderThread();

    void thread_func();

    uint64_t getCurrentTimestamp();

  private:
    SensorMng();

    std::vector<SensorData> m_sensors;

    std::thread m_thread;

    std::atomic<bool> m_bRunning;

    std::mutex m_lock;

    // Singleton instance
    static std::shared_ptr<SensorMng> m_instance;
};