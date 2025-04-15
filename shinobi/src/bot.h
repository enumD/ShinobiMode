#pragma once
#include "../utils/Logger.h"
#include <atomic>
#include <condition_variable>
#include <cstdlib> // per std::getenv
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <td/telegram/Client.h>
#include <td/telegram/Log.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

// overloaded
namespace detail
{
template <class... Fs> struct overload;

template <class F> struct overload<F> : public F
{
    explicit overload(F f) : F(f) {}
};
template <class F, class... Fs> struct overload<F, Fs...> : public overload<F>, public overload<Fs...>
{
    overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...) {}
    using overload<F>::operator();
    using overload<Fs...>::operator();
};
} // namespace detail

template <class... F> auto overloaded(F... f) { return detail::overload<F...>(f...); }

namespace td_api = td::td_api;
class Bot
{
  public:
    ~Bot();
    static std::shared_ptr<Bot> getInstance();
    // Delete copy and move constructors and assignment operators: Used to prevent Bot to be copied
    // and moved, because it is not thread safe
    Bot(const Bot &) = delete;
    Bot &operator=(const Bot &) = delete;
    Bot(Bot &&) = delete;
    Bot &operator=(Bot &&) = delete;
    // End of

    void start();
    void run();
    void shutdown();
    bool isRunning();
    void clearHandlers();
    std::future<td_api::object_ptr<td_api::message>> send_message_async(int64_t chat_id, const std::string &text, int64_t reply_to_message_id = 0);

  private:
    Bot();

    void send_query(td_api::object_ptr<td_api::Function> f, std::function<void(td_api::object_ptr<td_api::Object>)> handler);
    std::thread m_thread;
    std::atomic<bool> m_bRunning;
    std::mutex m_lock;

    int bot_user_id_;
    static std::shared_ptr<Bot> m_instance;
    std::unique_ptr<td::ClientManager> m_client_manager;
    std::int32_t m_client_id_{0};

    td_api::object_ptr<td_api::AuthorizationState> authorization_state_;
    bool are_authorized_{false};
    bool need_restart_{false};
    std::uint64_t current_query_id_{2};
    std::uint64_t authentication_query_id_{0};

    std::map<std::uint64_t, std::function<void(td_api::object_ptr<td_api::Object>)>> handlers_;

    std::map<std::int64_t, td_api::object_ptr<td_api::user>> users_;

    std::map<std::int64_t, std::string> chat_title_;

  private:
    std::uint64_t next_query_id() { return ++current_query_id_; }

    void check_authentication_error(td_api::object_ptr<td_api::Object> object);
    auto create_authentication_query_handler();

    void on_authorization_state_update();
    void process_update(td_api::object_ptr<td_api::Object> update);
    void process_response(td::ClientManager::Response response);

    std::string get_user_name(std::int64_t user_id) const
    {
        auto it = users_.find(user_id);
        if (it == users_.end())
        {
            return "unknown user";
        }
        return it->second->first_name_ + " " + it->second->last_name_;
    }

    std::string get_chat_title(std::int64_t chat_id) const
    {
        auto it = chat_title_.find(chat_id);
        if (it == chat_title_.end())
        {
            return "unknown chat";
        }
        return it->second;
    }

    std::future<int64_t> get_my_saved_messages_chat_id()
    {
        std::lock_guard<std::mutex> guard(m_lock);
        auto promise = std::make_shared<std::promise<int64_t>>();
        auto future = promise->get_future();

        // Passo 1: Ottieni l'utente corrente (tu stesso)
        auto get_me = td_api::make_object<td_api::getMe>();

        send_query(std::move(get_me),
                   [this, promise](td_api::object_ptr<td_api::Object> obj)
                   {
                       if (obj->get_id() == td_api::user::ID)
                       {
                           auto user = td::move_tl_object_as<td_api::user>(obj);
                           int64_t my_user_id = user->id_;

                           // Passo 2: Costruisci il chat_id personale
                           // Per TDLib, il chat con sé stessi è:  chat_id = user_id * 1'000'000'000'000 + user_id
                           int64_t my_chat_id = static_cast<int64_t>(my_user_id) * 1'000'000'000'000LL + my_user_id;
                           promise->set_value(my_chat_id);
                       }
                       else if (obj->get_id() == td_api::error::ID)
                       {
                           auto error = td::move_tl_object_as<td_api::error>(obj);
                           promise->set_exception(std::make_exception_ptr(std::runtime_error("getMe failed: " + error->message_)));
                       }
                   });

        return future;
    }
};