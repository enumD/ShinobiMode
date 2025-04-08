#pragma once

#include <imgui.h>
#include <string>
#include <vector>
#include <functional>
#include "../utils/myGlobals.h"

class ModeSelector
{
public:
      void init();

    void shutdown();

    void render();

    AlarmMode GetSelectedMode() const;

private:
    AlarmMode m_selectedMode = AlarmMode::NO_MODE;

    bool m_bInit = false;

    std::vector<std::pair<AlarmMode, bool>> m_modes; // List of modes (name, state)
};
