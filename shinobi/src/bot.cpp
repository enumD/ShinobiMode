#include "bot.h"
#include <iostream>

std::shared_ptr<Bot> Bot::m_instance = nullptr;
std::shared_ptr<Bot> Bot::getInstance()
{ // Create a new SensorMng only is not exist yet
    if (!m_instance)
    {
        td::Log::set_verbosity_level(4);
        td::Log::set_file_path("tdlib_log.txt");
        m_instance = std::shared_ptr<Bot>(new Bot());
    }
    return m_instance;
}

Bot::~Bot() { this->shutdown(); }

Bot::Bot() : m_clientMng(std::make_unique<td::ClientManager>()), m_client_id_(m_clientMng->create_client_id()), m_token(""), m_config()
{
    try
    {
        std::filesystem::create_directory("tdlib_db");
        std::filesystem::create_directory("tdlib_files");

        m_token = getTokenFromFile();

        std::ifstream config_file("tdlib_files/td_cinfig.json");
        if (!config_file.is_open())
        {
            throw std::runtime_error("Failed to open config file");
        }

        config_file >> m_config;

        m_clientMng->send(m_client_id_, 0, nullptr);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Config error: " + std::string(e.what()));
    }
}

void Bot::authenticateBot()
{
    if (m_token.empty())
        throw std::runtime_error("Token non caricato!");

    auto auth = td::td_api::make_object<td::td_api::checkAuthenticationBotToken>();
    auth->token_ = m_token;
    m_clientMng->send(m_client_id_, 0, std::move(auth));
}

void Bot::start()
{
    if (m_bRunning == false)
    {
        m_bRunning = true;

        std::lock_guard<std::mutex> lock(m_instance->m_shutdown_mutex);
        m_instance->m_shutdown_requested = false;

        // Start thread
        m_thread = std::thread(&Bot::run, this);

        std::cout << "Bot::start() - thread started\n" << std::endl;
    }
}

void Bot::shutdown()
{
    m_bRunning = false;

    if (m_instance)
    {
        {
            std::lock_guard<std::mutex> lock(m_instance->m_shutdown_mutex);
            m_instance->m_shutdown_requested = true;
        }
        m_instance->m_shutdown_cv.notify_all();
        m_instance.reset();
    }

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

void Bot::handleAuthorization(td::ClientManager::Response &response)
{
    auto update = td::td_api::move_object_as<td::td_api::updateAuthorizationState>(response.object);
    auto auth_state = std::move(update->authorization_state_);

    if (auth_state->get_id() == td::td_api::authorizationStateWaitTdlibParameters::ID)
    {
        auto params = td::td_api::make_object<td::td_api::setTdlibParameters>();

        params->api_id_ = m_config["api_id"];
        params->api_hash_ = m_config["api_hash"].get<std::string>();
        params->database_directory_ = m_config["database_directory"].get<std::string>();
        params->use_test_dc_ = m_config["use_test_dc"];
        params->files_directory_ = m_config["files_directory"].get<std::string>();
        params->system_language_code_ = m_config["system_language_code"].get<std::string>();
        params->device_model_ = m_config["device_model"].get<std::string>();
        params->application_version_ = m_config["application_version"].get<std::string>();
        params->use_message_database_ = true; // Opzionale, ma consigliato

        // After creating params in constructor:
        m_clientMng->send(m_client_id_, 0, std::move(params));
    }

    else if (auth_state->get_id() == td::td_api::authorizationStateWaitPhoneNumber::ID)
    {
        authenticateBot();
    }
    else if (auth_state->get_id() == td::td_api::authorizationStateReady::ID)
    {
        m_is_authorized = true;
        std::cout << "Authorization completed!" << std::endl;
    }
    else if (auth_state->get_id() == td::td_api::authorizationStateClosed::ID)
    {
        std::cout << "Authorization closed!" << std::endl;
        shutdown();
    }
    else
    {
        std::cerr << "Unhandled authorization state: " << auth_state->get_id() << std::endl;
    }
}

void Bot::sendMessage(int64_t chat_id, const std::string &text)
{
    auto send_message = td::td_api::make_object<td::td_api::sendMessage>();
    send_message->chat_id_ = chat_id;

    auto message_content = td::td_api::make_object<td::td_api::inputMessageText>();
    message_content->text_ = td::td_api::make_object<td::td_api::formattedText>();
    message_content->text_->text_ = text;

    send_message->input_message_content_ = std::move(message_content);

    m_clientMng->send(m_client_id_, 0, std::move(send_message));
}

void Bot::run()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(m_shutdown_mutex);
        if (m_shutdown_cv.wait_for(lock, std::chrono::seconds(1), [this] { return m_shutdown_requested.load(); }))
        {
            break;
        }

        auto response = m_clientMng->receive(10.0);

        if (!response.object)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        // Prima gestisci autorizzazione

        auto res = response.object->get_id();

        std::cout << "Ho ricevuto: " << res << " da : " << response.client_id << ". Con request: " << response.request_id << std::endl;

        if (response.object->get_id() == td::td_api::updateAuthorizationState::ID)
        {
            handleAuthorization(response);

            continue;
        }
        else if (response.object->get_id() == td::td_api::updateNewMessage::ID)
        {
            auto update = td::td_api::move_object_as<td::td_api::updateNewMessage>(response.object);
            auto &message = *update->message_;

            if (message.content_->get_id() == td::td_api::messageText::ID)
            {
                auto messageText = static_cast<td::td_api::messageText *>(message.content_.get());
                std::cout << "Received: " << messageText->text_->text_ << std::endl;

                sendMessage(message.chat_id_, "Ho ricevuto il tuo messaggio: " + messageText->text_->text_);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

std::string Bot::getTokenFromFile()
{
    std::ifstream tokenFile("token.txt");
    std::string token;

    if (!tokenFile.is_open())
    {
        throw std::runtime_error("Impossibile aprire il file del token: ");
    }

    // Legge la prima linea (dovrebbe contenere il token)
    std::getline(tokenFile, token);

    // Rimuove eventuali spazi bianchi o newline
    token.erase(token.find_last_not_of(" \n\r\t") + 1);

    if (token.empty())
    {
        throw std::runtime_error("Il file del token è vuoto o non valido");
    }

    return token;
}

bool Bot::isRunning() { return m_bRunning; }