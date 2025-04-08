#include "SensorData.h"

using namespace std::chrono;
using namespace date;

SensorData::SensorData() : m_status(0),
                           m_id(-1),
                           m_timeStamp(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()))
{
}

SensorData::SensorData(double Id, double Status)
{
    m_timeStamp = std::chrono::time_point_cast<std::chrono::seconds>(
        std::chrono::system_clock::now());
    m_id = Id;
    m_status = Status;
}

SensorData::SensorData(const SensorData &other)
{
    m_status = other.m_status;
    m_id = other.m_id;
    m_timeStamp = other.m_timeStamp;
}

void SensorData::SetStatus(double Status)
{
    m_status = Status;
}

SensorData::~SensorData()
{
}

SensorData &SensorData::operator=(const SensorData &other)
{
    if (this != &other)
    {
        this->m_status = other.m_status;
        this->m_id = other.m_id;
        this->m_timeStamp = other.m_timeStamp;
    }
    return *this;
}