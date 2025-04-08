// common.h
#pragma once
#include <string>
#include "date.h"

#define NUM_OF_SENSORS 4

#define THREAD_SLEEP_MILLI 500

enum class AlarmMode
{
    NO_MODE = 0,
    DOG_MODE,
    SENTINEL_MODE,
    SHINOBI_MODE,
    NUM_OF_ALARM_MODE
};

inline std::string AlarmMode_ToString(AlarmMode mode)
{
    switch (mode)
    {
    case AlarmMode::NO_MODE:
        return "NO MODE";
    case AlarmMode::DOG_MODE:
        return "DOG MODE";
    case AlarmMode::SENTINEL_MODE:
        return "SENTINEL MODE";
    case AlarmMode::SHINOBI_MODE:
        return "SHINOBI MODE";
    default:
        return "NUM_OF_ALARM_MODE";
    }

    return "NUM_OF_ALARM_MODE";
}
