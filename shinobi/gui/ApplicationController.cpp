#include "ApplicationController.h"

void ApplicationController::initialize() {
    m_mainWindow.initialize();
    m_optionScreen.Init();
    setupToolbar();
}

void ApplicationController::setupToolbar() {
    // Icona per la main window
    m_toolbar.AddIcon("home", "🏠", true, [this] {
        m_activeWindow = ActiveWindow::MAIN;
    });
    
    // Icona per le impostazioni
    m_toolbar.AddIcon("settings", "⚙️", true, [this] {
        m_activeWindow = ActiveWindow::OPTIONS;
        m_optionScreen.SetVisible(true);
    });
    
    // Icona per la fotocamera
    m_toolbar.AddIcon("camera", "📷", true, [this] {
        if (m_mainWindow.camera().isRunning()) {
            m_mainWindow.camera().stop();
        } else {
            m_mainWindow.camera().start(0);
        }
    });
}
void ApplicationController::render() {
    // 1. Render toolbar first (fixed position)
    m_toolbar.Draw();

    // 2. Calculate remaining space for content
    ImVec2 content_pos(0, 40); // Below toolbar
    ImVec2 content_size(
        ImGui::GetIO().DisplaySize.x,
        ImGui::GetIO().DisplaySize.y - 40
    );

    // 3. Render active window in content area
    ImGui::SetNextWindowPos(content_pos);
    ImGui::SetNextWindowSize(content_size);
    
    switch(m_activeWindow) {
        case ActiveWindow::MAIN:
            m_mainWindow.render();
            break;
        case ActiveWindow::OPTIONS: {
            bool open = true;
            ImGui::Begin("Settings", &open, 
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove);
            
            m_optionScreen.Draw(&open);
            if(!open) m_activeWindow = ActiveWindow::MAIN;
            
            ImGui::End();
            break;
        }
    }

 }
 
// void ApplicationController::render() {
//     // 1. Prima la finestra principale trasparente
//     ImGui::SetNextWindowPos(ImVec2(0, 0));
//     ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
//     ImGui::Begin("MainDockSpace", nullptr, 
//         ImGuiWindowFlags_NoTitleBar | 
//         ImGuiWindowFlags_NoResize | 
//         ImGuiWindowFlags_NoMove |
//         ImGuiWindowFlags_NoBackground |
//         ImGuiWindowFlags_NoBringToFrontOnFocus);

//     // 2. Crea uno spazio docking
//     ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
//     ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

//     // 3. Render della toolbar (sempre visibile)
//     ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_Always);
//     m_toolbar.Draw();

//     // 4. Render del contenuto dinamico
//     ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_Appearing);
//     switch(m_activeWindow) {
//         case ActiveWindow::MAIN:
//             m_mainWindow.render();
//             break;
//         case ActiveWindow::OPTIONS:
//             bool open = true;
//             m_optionScreen.Draw(&open);
//             if(!open) m_activeWindow = ActiveWindow::MAIN;
//             break;
//     }

//     ImGui::End();
// }
// void ApplicationController::render() {
//     // Render sempre la toolbar
//     m_toolbar.Draw();
    
//     // Render finestra attiva
//     switch (m_activeWindow) {
//         case ActiveWindow::MAIN:
//             m_mainWindow.render();
//             break;
//         case ActiveWindow::OPTIONS:
//             bool optionsOpen = true;
//             m_optionScreen.Draw(&optionsOpen);
//             if (!optionsOpen) {
//                 m_activeWindow = ActiveWindow::MAIN;
//             }
//             break;
//     }
// }

void ApplicationController::shutdown() {
    m_mainWindow.shutdown();
}