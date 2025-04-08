#pragma once
#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>
#include "../utils/Logger.h"
#include "../utils/myGlobals.h"

class Dog
{

public:
    Dog();

    void start();

    void stop();

    ~Dog();

private:
    void thread_func();

    std::thread m_thread;
    std::atomic<bool> m_bRunning;
};