#include "ModeSelector.h"

void ModeSelector::init()
{

    m_bInit = true;

    // Initialize available modes
    m_modes = {
        {AlarmMode::NO_MODE, false},
        {AlarmMode::DOG_MODE, false},
        {AlarmMode::SENTINEL_MODE, false},
        {AlarmMode::SHINOBI_MODE, false}};

    // Select first mode by default
    if (!m_modes.empty())
    {
        m_modes[0].second = true;
        m_selectedMode = m_modes[0].first;
    }
}

void ModeSelector::shutdown()
{

    m_selectedMode = AlarmMode::NO_MODE;
    m_bInit = false;
    m_modes.clear();
}

void ModeSelector::render()
{
    ImGui::BeginChild("Mode Selector", ImVec2(0, 0), true);

    // Title
    ImGui::Text("Select operational mode:");
    ImGui::Separator();

    // Increase the padding size (increasing padding makes the radio buttons appear larger)
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10)); // Increase padding
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 15));  // Più distanziati
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);                 // Usa il font di default
    ImGui::SetWindowFontScale(1.3f);                                 // 1.5x più grande

    // Radio buttons for modes
    for (int i = 0; i < m_modes.size(); ++i)
    {
        bool isSelected = ((int)m_selectedMode == i);

        std::string tmpString = AlarmMode_ToString(m_modes[i].first);

        if (ImGui::RadioButton(tmpString.c_str(), isSelected))
        {
            m_selectedMode = (AlarmMode)i;

            // Update all modes' states
            for (auto &mode : m_modes)
            {
                mode.second = false;
            }

            m_modes[i].second = true;
        }

        // Show state (On/Off)
        ImGui::SameLine();
        ImGui::TextColored(m_modes[i].second ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
                           m_modes[i].second ? "On" : "Off");
    }

    // Restore the original padding after the radio buttons
    ImGui::PopStyleVar(2);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopFont();
    ImGui::Separator();

    // Save button
    if (ImGui::Button("Save"))
    {
    }

    ImGui::EndChild();
}

AlarmMode ModeSelector::GetSelectedMode() const
{
    return m_selectedMode;
}
