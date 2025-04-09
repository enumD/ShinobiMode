
#pragma once
#include "date.h"
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
/// WRITE BY CHATGPT

class Logger
{
  public:
    static std::string getCurrentDateTime();

    static void log(const std::string &message, bool isError = false);

  private:
    // Mutex per gestire l'accesso concorrente al file di log
    inline static std::mutex mtx;
};
