#pragma once
#include "Modes.h"
#include "myBot.h"

class Sentinel : public Modes
{

  public:
    explicit Sentinel();

    ~Sentinel() override;

    // Delete copy and move constructors and assignment operators: Used to prevent Dog to be copied
    // and moved, because it is not thread safe
    Sentinel(const Sentinel &) = delete;
    Sentinel &operator=(const Sentinel &) = delete;
    Sentinel(Sentinel &&) = delete;
    Sentinel &operator=(Sentinel &&) = delete;
    void start() override;
    void stop() override;
    // End of

  private:
    void _thread_func() override;
    std::shared_ptr<MyBot> m_pBot;

    void _signalToTelegram();
    void _autoMode();
    int m_countTo = 6; // 6 -> 3 seconds - 12 -> 6 seconds
    std::vector<int> m_vCount;
};