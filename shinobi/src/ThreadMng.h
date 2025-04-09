#pragma once
#include "../interface/IStartStop.h"
#include "../utils/myGlobals.h"
#include "SensorMng.h"
#include "dog.h"
#include <memory>
#include <vector>

/// @brief Init the sensormng and the alarm mode classes and manage their behaviours
class ThreadMng
{

  public:
    ThreadMng();
    ~ThreadMng();

    void Init();
    void Terminate();
    void SetThread(AlarmMode Mode);

  private:
    std::vector<std::pair<AlarmMode, IStartStop &>> m_threads;
    SensorMng m_sensorMng;
    Dog m_dog;
};