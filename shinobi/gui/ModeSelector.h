#pragma once

#include "../src/ThreadMng.h"
#include "../utils/myGlobals.h"
#include <functional>
#include <imgui.h>
#include <string>
#include <vector>

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

    ThreadMng m_threadMng;
};
