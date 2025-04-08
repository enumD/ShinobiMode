#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include "date.h"
/// WRITE BY CHATGPT

class Logger
{
public:
    static std::string getCurrentDateTime()
    {
        using namespace std::chrono;
        using namespace date;

        auto now = system_clock::now();
        auto time = system_clock::to_time_t(now);

        // Utilizza la libreria date.h per formattare la data e l'ora
        std::ostringstream oss;
        oss << format("%Y-%m-%d %H:%M:%S", floor<seconds>(now)); // Formatta la data e l'ora
        return oss.str();
    }

    static void log(const std::string &message, bool isError = false)
    {
        std::lock_guard<std::mutex> guard(mtx);               // Locking per la sicurezza in multithreading
        std::ofstream logFile("log.txt", std::ios_base::app); // Apre il file in modalità append
        if (logFile.is_open())
        {
            std::string currentDateTime = getCurrentDateTime();
            if (isError)
            {
                logFile << currentDateTime << " ERRORE - " << message << std::endl; // Logga come errore
            }
            else
            {
                logFile << currentDateTime << " " << message << std::endl; // Logga normalmente
            }
        }
        else
        {
            std::cerr << "Impossibile aprire il file di log!" << std::endl;
        }
    }

private:
    // Mutex per gestire l'accesso concorrente al file di log
    static std::mutex mtx;
};

// Definizione del mutex statico
std::mutex Logger::mtx;
