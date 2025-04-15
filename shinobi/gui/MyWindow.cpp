#include "MyWindow.h"
#include "../utils/Notification.h"
#include "imgui.h"

void MyWindow::initialize()
{
    setupToolbar();

    m_cameraWindow.initialize();

    m_settingsWindow.initialize();

    m_modeSelector.init();
}

void MyWindow::setupToolbar()
{
    // Icon on toolbar are fixed ordering, to ensure fast looking for something on or no:
    // Add
    m_toolbar.AddIcon("mode", "SHINOBI ON", true, [this] { Notification::Show("To enable/disable a mode please use the relative window", Notification::Level::INFO); });

    m_toolbar.AddIcon("sensor",
                      "SENSOR MNG",
                      true,
                      [this]
                      {
                          auto sensorMng = SensorMng::getInstance();
                          if (sensorMng)
                          {
                              if (sensorMng->isRunning())
                              {
                                  sensorMng->stop();
                                  Notification::Show("Sensor Disabled", Notification::Level::INFO);
                                  std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                  m_toolbar.SetIconBgColor("sensor", ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                              }
                              else
                              {
                                  sensorMng->start();
                                  Notification::Show("Sensor Enabled", Notification::Level::SUCCESS);
                                  std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                  m_toolbar.SetIconBgColor("sensor", ImVec4(0.0f, 0.5f, 0.0f, 0.6f));
                              }
                          }
                      });

    // Sensor Mng Start with ThreadMng for now so keep it green TODO
    m_toolbar.SetIconBgColor("sensor", ImVec4(0.0f, 0.5f, 0.0f, 0.6f));
}

void MyWindow::render()
{
    // Finestra principale che occupa tutto lo schermo
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("My Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);


    renderToolbar();

    const float toolbar_height = 40.0f;
    ImGui::BeginChild("Toolbar", ImVec2(0, toolbar_height), false, ImGuiWindowFlags_NoScrollbar);
    m_toolbar.Draw();
    ImGui::EndChild();

    // 2. Area sotto la toolbar (restante altezza)
    ImGui::BeginChild("ContentArea", ImVec2(0, -1), false, ImGuiWindowFlags_NoScrollbar);
    {
        // 3. Menu laterale fisso (50px)
        ImGui::BeginChild("LeftMenu", ImVec2(50, -1), true);
        renderMenu();
        ImGui::EndChild();

        // 4. Area contenuto adattabile (restante larghezza)
        ImGui::SameLine();
        ImGui::BeginChild("DynamicContent", ImVec2(-1, -1), true);
        renderContent();
        ImGui::EndChild();
    }
    ImGui::EndChild(); // ContentArea

    ImGui::End(); // End MyWindow
}


void MyWindow::renderToolbar()
{
    auto modeSelected = m_modeSelector.GetSelectedMode();
    if (modeSelected != m_lastMode)
    {
        m_lastMode = modeSelected;

        if (modeSelected == AlarmMode::NO_MODE || modeSelected == AlarmMode::NUM_OF_ALARM_MODE)
        {
            m_toolbar.SetIconText("mode", "NO MODE");
            m_toolbar.SetIconTextColor("mode", ImVec4(1, 1, 1, 1));
            m_toolbar.SetIconBgColor("mode", ImVec4(0, 0, 0, 0));
        }
        else if (modeSelected == AlarmMode::DOG_MODE)
        {
            m_toolbar.SetIconText("mode", "DOG MODE");
            m_toolbar.SetIconTextColor("mode", ImVec4(1, 1, 1, 1));
            m_toolbar.SetIconBgColor("mode", ImVec4(0.0f, 0.5f, 0.0f, 0.6f));
        }
        else if (modeSelected == AlarmMode::SENTINEL_MODE)
        {
            m_toolbar.SetIconText("mode", "SENTINELL MODE");
            m_toolbar.SetIconTextColor("mode", ImVec4(1, 1, 1, 1));

            m_toolbar.SetIconBgColor("mode", ImVec4(0.0f, 0.5f, 0.0f, 0.6f));
        }
        else if (modeSelected == AlarmMode::SHINOBI_MODE)
        {
            m_toolbar.SetIconText("mode", "SHINOBI MODE");
            m_toolbar.SetIconTextColor("mode", ImVec4(1, 1, 1, 1));

            m_toolbar.SetIconBgColor("mode", ImVec4(0.0f, 0.5f, 0.0f, 0.6f));
        }
    }
}


void MyWindow::renderMenu()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 20));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 15));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 15));

    // Bottoni verticali centrati
    ImGui::SetCursorPosX((50 - 40) * 0.5f); // Centra in 50px

    if (ImGui::Button("📷", ImVec2(40, 40)))
    {
        m_activeWindow = ActiveWindow::MAIN;
    }

    ImGui::SetCursorPosX((50 - 40) * 0.5f);

    if (ImGui::Button("⚙️", ImVec2(40, 40)))
    {
        m_activeWindow = ActiveWindow::SETTINGS;
    }

    ImGui::SetCursorPosX((50 - 40) * 0.5f);

    if (ImGui::Button("666", ImVec2(40, 40)))
    {
        m_activeWindow = ActiveWindow::MODE_SELECTOR;
    }

    ImGui::PopStyleVar(3);
}

void MyWindow::renderContent()
{
    switch (m_activeWindow)
    {
    case ActiveWindow::MAIN:
        m_cameraWindow.render();
        break;
    case ActiveWindow::SETTINGS:
        m_settingsWindow.render();
        break;
    case ActiveWindow::MODE_SELECTOR:
        m_modeSelector.render();
        break;
    case ActiveWindow::NONE:
        ImGui::Text("Seleziona una funzione dal menu a destra");
        break;
    }
}

void MyWindow::shutdown()
{
    m_cameraWindow.shutdown();
    m_settingsWindow.shutdown();
    m_modeSelector.shutdown();
}