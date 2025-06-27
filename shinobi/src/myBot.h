#pragma once

#include "../utils/Logger.h"
#include <atomic>
#include <curl/curl.h>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <thread>


class MyBot
{
  public:
    static std::shared_ptr<MyBot> getInstance();

    ~MyBot();

    // Delete copy and move constructors and assignment operators: Used to prevent Bot to be copied
    // and moved, because it is not thread safe
    MyBot(const MyBot &) = delete;
    MyBot &operator=(const MyBot &) = delete;
    MyBot(MyBot &&) = delete;
    MyBot &operator=(MyBot &&) = delete;
    // End of

    std::future<std::string> SendMessageAsync(const std::string &text);

    void start();
    void stop();
    bool isRunning();
    void deleteWaitingReplyFromSender();

  private:
    MyBot();

    std::string m_chatId;
    std::string m_botToken;
    int64_t last_update_id_;
    std::map<std::string, std::promise<std::string>> waiting_replies_; // chat_id -> promise

    std::thread m_thread;
    std::atomic<bool> m_bRunning;
    std::mutex m_lock;
    static std::shared_ptr<MyBot> m_instance;

    void thread_func();
    static size_t writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
    bool sendMessageInternal(const std::string &chat_id, const std::string &text);
    std::string getUpdatesInternal();
    void saveLastUpdateToFile(const std::string &filename);
    void pollingLoop();

    std::string getTokenFromFile();
};
