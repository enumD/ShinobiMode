#pragma once
#include "Toolbar.h"
#include "MainWindow.h"
#include "OptionScreen.h"

class ApplicationController {
public:
    void initialize();
    void render();
    void shutdown();

private:
    Toolbar m_toolbar;
    MainWindow m_mainWindow;
    OptionScreen m_optionScreen;
    
    enum class ActiveWindow { MAIN, OPTIONS };
    ActiveWindow m_activeWindow = ActiveWindow::MAIN;
    
    void setupToolbar();
};