#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include "imgui.h"
#include <string>

class MessageBox {
public:
    static void Show(const std::string& message);
};

#endif // MESSAGEBOX_H
