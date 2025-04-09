// Logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include "myGlobals.h"

class SensorData
{

  public:
    SensorData();

    SensorData(double Id, double Status);

    SensorData(const SensorData &other);

    void SetStatus(double Status);

    ~SensorData();

    SensorData &operator=(const SensorData &other);

    // 0 mean not active
    double m_status;
    double m_id;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> m_timeStamp;
};

#endif // LOGGER_H