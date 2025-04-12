#pragma once
#include "../utils/Logger.h"
#include <atomic>
#include <condition_variable>
#include <cstdlib> // per std::getenv
#include <fstream>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>
class Bot
{
  public:
    static std::shared_ptr<Bot> getInstance();
    ~Bot();

    // Delete copy and move constructors and assignment operators: Used to prevent Bot to be copied
    // and moved, because it is not thread safe
    Bot(const Bot &) = delete;
    Bot &operator=(const Bot &) = delete;
    Bot(Bot &&) = delete;
    Bot &operator=(Bot &&) = delete;
    // End of

    std::string getTokenFromFile();

    void sendMessage(int64_t chat_id, const std::string &text);
    void handleAuthorization(td::ClientManager::Response &response);
    void authenticateBot();

    void start();
    void shutdown();
    bool isRunning();

  private:
    void run();
    Bot();

    std::unique_ptr<td::ClientManager> m_clientMng;
    int32_t m_client_id_;
    std::string m_token;

    // Shutdown-related members
    std::atomic<bool> m_shutdown_requested{false};
    std::mutex m_shutdown_mutex;
    std::condition_variable m_shutdown_cv;

    // Singleton instance
    static std::shared_ptr<Bot> m_instance;
    std::atomic<bool> m_is_authorized{false};
    std::thread m_thread;
    std::atomic<bool> m_bRunning;
};