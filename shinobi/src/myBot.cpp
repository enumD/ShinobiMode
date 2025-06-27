#include "myBot.h"


using json = nlohmann::json;

std::shared_ptr<MyBot> MyBot::m_instance = nullptr;

MyBot::MyBot() : m_botToken(getTokenFromFile()), last_update_id_(0), m_bRunning(false), m_chatId("7097571880") {}

MyBot::~MyBot()
{
    this->stop();


    if (m_instance)
    {
        m_instance.reset();
    }
}

std::shared_ptr<MyBot> MyBot::getInstance()
{
    // Create a new bot only is not exist yet
    if (!m_instance)
    {
        m_instance = std::shared_ptr<MyBot>(new MyBot());
    }
    return m_instance;
}

void MyBot::start()
{
    if (m_bRunning == false)
    {
        m_bRunning = true;

        curl_global_init(CURL_GLOBAL_DEFAULT);

        // Start thread
        m_thread = std::thread(&MyBot::thread_func, this);

        std::cout << "Bot::start() - thread started\n" << std::endl;
    }
}

void MyBot::stop()
{
    m_bRunning = false;

    try
    {
        if (m_thread.joinable())
        {
            m_thread.join();

            waiting_replies_.clear();

            curl_global_cleanup();

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

bool MyBot::isRunning() { return m_bRunning; }

void MyBot::deleteWaitingReplyFromSender()
{
    std::lock_guard<std::mutex> guard(m_lock);
    waiting_replies_.erase(m_chatId);
}

void MyBot::thread_func()
{
    try
    {
        while (m_bRunning)
        {
            pollingLoop();

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        Logger::log(e.what());
    }
}


void MyBot::pollingLoop()
{

    std::string response = getUpdatesInternal();

    try
    {
        auto parsed = json::parse(response);
        if (parsed.contains("result"))
        {
            for (auto &update : parsed["result"])
            {
                last_update_id_ = update["update_id"].get<int64_t>();
                if (update.contains("message"))
                {
                    auto msg = update["message"];
                    std::string text = msg["text"];
                    auto tete = msg["chat"]["id"];
                    m_chatId = tete.is_string() ? tete.get<std::string>() : std::to_string(tete.get<int64_t>());
                    std::cerr << "Chat ID is: " << m_chatId << " Message: " << text << std::endl;

                    std::lock_guard<std::mutex> guard(m_lock);
                    auto it = waiting_replies_.find(m_chatId);
                    if (it != waiting_replies_.end())
                    {
                        it->second.set_value(text);
                        waiting_replies_.erase(it);
                    }
                }
            }
        }
    }
    catch (...)
    {
        std::cerr << "Errore parsing JSON\n";
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}


std::string MyBot::getUpdatesInternal()
{
    std::string response;
    CURL *curl = curl_easy_init();
    if (curl)
    {
        std::ostringstream url;
        url << "https://api.telegram.org/bot" << m_botToken << "/getUpdates?timeout=10&offset=" << (last_update_id_ + 1);

        curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &MyBot::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return response;
}


size_t MyBot::writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::string *response = static_cast<std::string *>(userdata);
    response->append(ptr, size * nmemb);
    return size * nmemb;
}


std::future<std::string> MyBot::SendMessageAsync(const std::string &text)
{
    std::promise<std::string> promise;

    auto future = promise.get_future();

    {
        std::lock_guard<std::mutex> guard(m_lock);

        waiting_replies_.erase(m_chatId); // reset
    }

    if (sendMessageInternal(m_chatId, text))
    {
        std::lock_guard<std::mutex> guard(m_lock);
        waiting_replies_[m_chatId] = std::move(promise);
        // waiting_replies_[m_chatId] = std::move(*promise);
    }
    else
    {
        promise.set_value("");
    }

    return future;
}


bool MyBot::sendMessageInternal(const std::string &chat_id, const std::string &text)
{
    bool bRes = false;
    std::string response;
    CURL *curl = curl_easy_init();
    if (curl)
    {
        std::ostringstream url;
        url << "https://api.telegram.org/bot" << m_botToken << "/sendMessage";

        // Properly escape all components
        char *escaped_text = curl_easy_escape(curl, text.c_str(), text.length());
        char *escaped_chat_id = curl_easy_escape(curl, chat_id.c_str(), chat_id.length());

        std::string post_fields = "chat_id=" + std::string(escaped_chat_id) + "&text=" + std::string(escaped_text);

        curl_free(escaped_text);
        curl_free(escaped_chat_id);

        std::cerr << "Sending: " << text << ". To chat id: " << chat_id << std::endl;

        // Set curl options
        curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &MyBot::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Add headers
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        CURLcode res = curl_easy_perform(curl);

        // DEBUG
        if (res == CURLE_OK)
        {
            std::cerr << "Response: " << response << std::endl;
            bRes = true;
        }
        else
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        // END DEBUG

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return bRes;
}


// std::string MyBot::sendMessageInternal(const std::string &chat_id, const std::string &text)
// {
//     std::string response;
//     CURL *curl = curl_easy_init();
//     if (curl)
//     {
//         std::ostringstream url;
//         url << "https://api.telegram.org/bot" << m_botToken << "/sendMessage";

//         std::ostringstream data;
//         data << "chat_id=" << chat_id << "&text=" << curl_easy_escape(curl, text.c_str(), text.length());
//         std::cerr << "Sending: " << text << ". To chat id: " << chat_id << std::endl;

//         curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
//         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.str().c_str());
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &MyBot::writeCallback);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

//         CURLcode res = curl_easy_perform(curl);
//         if (res != CURLE_OK)
//         {
//             std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
//         }
//         else
//         {
//             std::cerr << "Response: " << response << std::endl;
//         }
//         curl_easy_cleanup(curl);
//     }
//     return response;
// }

void MyBot::saveLastUpdateToFile(const std::string &response)
{

    if (response.empty() == false)
    {
        std::ofstream outFile("jsooon.txt");
        if (outFile.is_open())
        {
            outFile << response;
            outFile.close();
            std::cout << "Update saved to "
                      << "json.txt" << std::endl;
        }
        else
        {
            std::cerr << "Failed to open file "
                      << "json.txt"
                      << " for writing." << std::endl;
        }
    }
}


std::string MyBot::getTokenFromFile()
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