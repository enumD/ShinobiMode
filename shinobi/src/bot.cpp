#include "bot.h"
#include <iostream>

std::shared_ptr<Bot> Bot::m_instance = nullptr;

Bot::~Bot()
{
    this->shutdown();

    if (m_client_manager)
    {
        send_query(td_api::make_object<td_api::close>(), {});
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        m_client_manager.reset();
    }

    this->clearHandlers();
    users_.clear();
    chat_title_.clear();

    if (m_instance)
    {
        m_instance.reset();
    }
}

Bot::Bot()
{
    td::ClientManager::execute(td_api::make_object<td_api::setLogVerbosityLevel>(1));
    m_client_manager = std::make_unique<td::ClientManager>();
    m_client_id_ = m_client_manager->create_client_id();
    send_query(td_api::make_object<td_api::getOption>("version"), {});
}

std::shared_ptr<Bot> Bot::getInstance()
{
    // Create a new SensorMng only is not exist yet
    if (!m_instance)
    {
        m_instance = std::shared_ptr<Bot>(new Bot());
    }
    return m_instance;
}

void Bot::start()
{
    if (m_bRunning == false)
    {
        m_bRunning = true;

        // Start thread
        m_thread = std::thread(&Bot::run, this);

        std::cout << "Bot::start() - thread started\n" << std::endl;
    }
}

void Bot::shutdown()
{
    m_bRunning = false;

    try
    {
        if (m_thread.joinable())
        {
            m_thread.join();

            std::cout << "Bot"
                      << "::stop() - thread stopped \n"
                      << std::endl;
        }
        else
        {
            std::cout << "Bot"
                      << "::stop() - thread was not joinable \n"
                      << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        Logger::log(e.what(), true);
    }
}

bool Bot::isRunning() { return m_bRunning; }

void Bot::run()
{

    try
    {
        while (m_bRunning)
        {
            process_response(m_client_manager->receive(10));

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        // TODO
        send_query(td_api::make_object<td_api::close>(), {});
        // send_query(td_api::make_object<td_api::logOut>(), {});
        // END TODO
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Bot::clearHandlers()
{
    std::lock_guard<std::mutex> guard(m_lock);
    handlers_.clear();
}

// bot.cpp
std::future<td_api::object_ptr<td_api::message>> Bot::send_message_async(int64_t chat_id, const std::string &text, int64_t reply_to_message_id)
{

    std::lock_guard<std::mutex> guard(m_lock);
    auto promise = std::make_shared<std::promise<td_api::object_ptr<td_api::message>>>();
    auto future = promise->get_future();

    auto send_message = td_api::make_object<td_api::sendMessage>();
    send_message->chat_id_ = chat_id;
    auto message_content = td_api::make_object<td_api::inputMessageText>();
    message_content->text_ = td_api::make_object<td_api::formattedText>();
    message_content->text_->text_ = std::move(text);
    send_message->input_message_content_ = std::move(message_content);
    if (reply_to_message_id != 0)
    {
        auto reply_to = td_api::make_object<td_api::inputMessageReplyToMessage>();
        reply_to->message_id_ = reply_to_message_id;
        send_message->reply_to_ = std::move(reply_to);
    }

    // Usa la tua send_query esistente
    send_query(std::move(send_message),
               [promise](td_api::object_ptr<td_api::Object> obj)
               {
                   if (obj->get_id() == td_api::message::ID)
                   {
                       promise->set_value(td::move_tl_object_as<td_api::message>(obj));
                   }
                   else if (obj->get_id() == td_api::error::ID)
                   {
                       auto error = td::move_tl_object_as<td_api::error>(obj);
                       promise->set_exception(std::make_exception_ptr(std::runtime_error(error->message_)));
                   }
               });

    return future;
}
void Bot::send_query(td_api::object_ptr<td_api::Function> f, std::function<void(td_api::object_ptr<td_api::Object>)> handler)
{
    auto query_id = next_query_id();
    if (handler)
    {
        handlers_.emplace(query_id, std::move(handler));
    }
    m_client_manager->send(m_client_id_, query_id, std::move(f));
}

void Bot::check_authentication_error(td_api::object_ptr<td_api::Object> object)
{
    if (object->get_id() == td_api::error::ID)
    {
        auto error = td::move_tl_object_as<td_api::error>(object);
        std::cout << "Error: " << to_string(error) << std::flush;
        on_authorization_state_update();
    }
}

auto Bot::create_authentication_query_handler()
{
    return [this, id = authentication_query_id_](td_api::object_ptr<td_api::Object> object)
    {
        if (id == authentication_query_id_)
        {
            check_authentication_error(std::move(object));
        }
    };
}

void Bot::on_authorization_state_update()
{
    authentication_query_id_++;
    td_api::downcast_call(*authorization_state_,
                          overloaded(
                              [this](td_api::authorizationStateReady &)
                              {
                                  are_authorized_ = true;
                                  std::cout << "Authorization is completed" << std::endl;
                              },
                              [this](td_api::authorizationStateLoggingOut &)
                              {
                                  are_authorized_ = false;
                                  std::cout << "Logging out" << std::endl;
                              },
                              [this](td_api::authorizationStateClosing &) { std::cout << "Closing" << std::endl; },
                              [this](td_api::authorizationStateClosed &)
                              {
                                  are_authorized_ = false;
                                  need_restart_ = true;
                                  std::cout << "Terminated" << std::endl;
                              },
                              [this](td_api::authorizationStateWaitPhoneNumber &)
                              {
                                  std::cout << "Enter phone number: " << std::flush;
                                  std::string phone_number;
                                  std::cin >> phone_number;
                                  send_query(td_api::make_object<td_api::setAuthenticationPhoneNumber>(phone_number, nullptr), create_authentication_query_handler());
                              },
                              [this](td_api::authorizationStateWaitEmailAddress &)
                              {
                                  std::cout << "Enter email address: " << std::flush;
                                  std::string email_address;
                                  std::cin >> email_address;
                                  send_query(td_api::make_object<td_api::setAuthenticationEmailAddress>(email_address), create_authentication_query_handler());
                              },
                              [this](td_api::authorizationStateWaitEmailCode &)
                              {
                                  std::cout << "Enter email authentication code: " << std::flush;
                                  std::string code;
                                  std::cin >> code;
                                  send_query(td_api::make_object<td_api::checkAuthenticationEmailCode>(td_api::make_object<td_api::emailAddressAuthenticationCode>(code)), create_authentication_query_handler());
                              },
                              [this](td_api::authorizationStateWaitCode &)
                              {
                                  std::cout << "Enter authentication code: " << std::flush;
                                  std::string code;
                                  std::cin >> code;
                                  send_query(td_api::make_object<td_api::checkAuthenticationCode>(code), create_authentication_query_handler());
                              },
                              [this](td_api::authorizationStateWaitRegistration &)
                              {
                                  std::string first_name;
                                  std::string last_name;
                                  std::cout << "Enter your first name: " << std::flush;
                                  std::cin >> first_name;
                                  std::cout << "Enter your last name: " << std::flush;
                                  std::cin >> last_name;
                                  send_query(td_api::make_object<td_api::registerUser>(first_name, last_name, false), create_authentication_query_handler());
                              },
                              [this](td_api::authorizationStateWaitPassword &)
                              {
                                  std::cout << "Enter authentication password: " << std::flush;
                                  std::string password;
                                  std::getline(std::cin, password);
                                  send_query(td_api::make_object<td_api::checkAuthenticationPassword>(password), create_authentication_query_handler());
                              },
                              [this](td_api::authorizationStateWaitOtherDeviceConfirmation &state) { std::cout << "Confirm this login link on another device: " << state.link_ << std::endl; },
                              [this](td_api::authorizationStateWaitTdlibParameters &)
                              {
                                  auto request = td_api::make_object<td_api::setTdlibParameters>();
                                  request->database_directory_ = "tdlib";
                                  request->use_message_database_ = true;
                                  request->use_secret_chats_ = true;
                                  request->api_id_ = 94575;                                // Sostituisci con il tuo API ID
                                  request->api_hash_ = "a3406de8d171bb422bb6ddf3bbd800e2"; // Sostituisci con il tuo API HASH
                                  request->system_language_code_ = "en";
                                  request->device_model_ = "Desktop";
                                  request->application_version_ = "1.0";
                                  send_query(std::move(request), create_authentication_query_handler());
                              },
                              [](td_api::authorizationStateWaitPremiumPurchase &) { std::cout << "Premium account required" << std::endl; },
                              [](auto &unknown_state) { std::cout << "Unhandled authorization state: " << td::td_api::to_string(unknown_state) << std::endl; }));
}

void Bot::process_update(td_api::object_ptr<td_api::Object> update)
{
    td_api::downcast_call(*update,
                          overloaded(
                              [this](td_api::updateAuthorizationState &update_authorization_state)
                              {
                                  authorization_state_ = std::move(update_authorization_state.authorization_state_);
                                  on_authorization_state_update();
                                  std::cout << "Receive updateAuthorizationState" << std::endl;
                              },
                              [this](td_api::updateNewChat &update_new_chat)
                              {
                                  chat_title_[update_new_chat.chat_->id_] = update_new_chat.chat_->title_;
                                  std::cout << "Receive updateNewChat" << std::endl;
                              },
                              [this](td_api::updateChatTitle &update_chat_title)
                              {
                                  chat_title_[update_chat_title.chat_id_] = update_chat_title.title_;
                                  std::cout << "Receive updateChatTitle" << std::endl;
                              },
                              [this](td_api::updateUser &update_user)
                              {
                                  auto user_id = update_user.user_->id_;
                                  users_[user_id] = std::move(update_user.user_);
                                  std::cout << "Receive update" << std::endl;
                              },
                              [this](td_api::updateNewMessage &update_new_message)
                              {
                                  auto chat_id = update_new_message.message_->chat_id_;
                                  std::string sender_name;
                                  td_api::downcast_call(*update_new_message.message_->sender_id_,
                                                        overloaded([this, &sender_name](td_api::messageSenderUser &user) { sender_name = get_user_name(user.user_id_); },
                                                                   [this, &sender_name](td_api::messageSenderChat &chat) { sender_name = get_chat_title(chat.chat_id_); }));
                                  std::string text;
                                  if (update_new_message.message_->content_->get_id() == td_api::messageText::ID)
                                  {
                                      text = static_cast<td_api::messageText &>(*update_new_message.message_->content_).text_->text_;
                                  }
                                  std::cout << "Receive message: [chat_id:" << chat_id << "] [from:" << sender_name << "] [" << text << "]" << std::endl;
                              },
                              [](auto &update) { std::cout << "Received update not managed" << std::endl; }));
}

void Bot::process_response(td::ClientManager::Response response)
{

    if (!response.object)
    {
        return;
    }
    std::cout << response.request_id << " " << to_string(response.object) << std::endl;
    if (response.request_id == 0)
    {
        return process_update(std::move(response.object));
    }

    std::lock_guard<std::mutex> guard(m_lock);
    // Check if it's a response to handle
    auto it = handlers_.find(response.request_id);
    if (it != handlers_.end())
    {
        it->second(std::move(response.object));
        handlers_.erase(it);
    }
}
