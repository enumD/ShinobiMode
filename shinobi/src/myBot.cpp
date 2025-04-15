#include "myBot.h"


using json = nlohmann::json;

std::shared_ptr<MyBot> MyBot::m_instance = nullptr;

MyBot::MyBot() : m_botToken(getTokenFromFile()), last_update_id_(0), m_bRunning(false), m_chatId("7730902880") {}

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
                    m_chatId = tete.is_string() ? tete.get<std::string>() : std::to_string(tete.get<int>());
                    std::cerr << "Chat ID is: " << m_chatId << std::endl;

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


std::string MyBot::sendMessageInternal(const std::string &chat_id, const std::string &text)
{
    std::string response;
    CURL *curl = curl_easy_init();
    if (curl)
    {
        std::ostringstream url;
        url << "https://api.telegram.org/bot" << m_botToken << "/sendMessage";

        std::ostringstream data;
        data << "chat_id=" << chat_id << "&text=" << curl_easy_escape(curl, text.c_str(), text.length());

        curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.str().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &MyBot::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return response;
}


std::future<std::string> MyBot::SendMessageAsync(const std::string &text)
{
    {
        std::lock_guard<std::mutex> guard(m_lock);
        waiting_replies_.erase(m_chatId); // reset
    }

    auto promise = std::make_shared<std::promise<std::string>>();
    auto future = promise->get_future();

    {
        std::lock_guard<std::mutex> guard(m_lock);
        waiting_replies_[m_chatId] = std::move(*promise);
    }

    sendMessageInternal(m_chatId, text);
    return future;
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