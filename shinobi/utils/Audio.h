// AudioManager.h
#pragma once
#include <string>
#include <pulse/pulseaudio.h>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    static float GetVolume();
    static void SetVolume(float level); // 0.0 (muto) a 1.0 (massimo)
    static void ToggleMute();
    
private:
    static pa_mainloop* m_mainloop;
    static pa_context* m_context;
    static float m_currentVolume;
    static bool m_isMuted;
    
    static void ContextStateCallback(pa_context* c, void* userdata);
    static void SinkInfoCallback(pa_context* c, const pa_sink_info* i, int eol, void* userdata);
    static void SuccessCallback(pa_context* c, int success, void* userdata);
};