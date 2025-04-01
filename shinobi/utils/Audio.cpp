// AudioManager.cpp
#include "Audio.h"
#include <thread>
#include <iostream>

pa_mainloop* AudioManager::m_mainloop = nullptr;
pa_context* AudioManager::m_context = nullptr;
float AudioManager::m_currentVolume = 0.7f;
bool AudioManager::m_isMuted = false;

AudioManager::AudioManager() {
    m_mainloop = pa_mainloop_new();
    m_context = pa_context_new(pa_mainloop_get_api(m_mainloop), "ImGui Audio Control");
    
    pa_context_set_state_callback(m_context, ContextStateCallback, nullptr);
    pa_context_connect(m_context, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
    
    // Avvia il thread per il mainloop
    std::thread([](){
        int ret;
        if (pa_mainloop_run(m_mainloop, &ret) < 0) {
            std::cerr << "PulseAudio mainloop error" << std::endl;
        }
    }).detach();
}

AudioManager::~AudioManager() {
    if (m_context) {
        pa_context_disconnect(m_context);
        pa_context_unref(m_context);
    }
    if (m_mainloop) {
        pa_mainloop_free(m_mainloop);
    }
}

void AudioManager::ContextStateCallback(pa_context* c, void* userdata) {
    switch (pa_context_get_state(c)) {
        case PA_CONTEXT_READY:
            pa_context_get_sink_info_by_name(c, "@DEFAULT_SINK@", SinkInfoCallback, nullptr);
            break;
        case PA_CONTEXT_FAILED:
        case PA_CONTEXT_TERMINATED:
            break;
        default:
            break;
    }
}

void AudioManager::SinkInfoCallback(pa_context* c, const pa_sink_info* i, int eol, void* userdata) {
    if (eol < 0) return;
    if (!i) return;
    
    m_currentVolume = static_cast<float>(pa_cvolume_avg(&i->volume) / PA_VOLUME_NORM);
    m_isMuted = i->mute;
}

void AudioManager::SetVolume(float level) {
    if (!m_context || level < 0.0f || level > 1.0f) return;
    
    pa_volume_t vol = static_cast<pa_volume_t>(level * PA_VOLUME_NORM);
    pa_cvolume cvol;
    pa_cvolume_set(&cvol, 2, vol); // 2 canali (stereo)
    
    pa_operation* op = pa_context_set_sink_volume_by_name(
        m_context, "@DEFAULT_SINK@", &cvol, SuccessCallback, nullptr);
    if (op) pa_operation_unref(op);
    
    m_currentVolume = level;
    m_isMuted = (level == 0.0f);
}

void AudioManager::ToggleMute() {
    if (!m_context) return;
    
    pa_operation* op = pa_context_set_sink_mute_by_name(
        m_context, "@DEFAULT_SINK@", !m_isMuted, SuccessCallback, nullptr);
    if (op) pa_operation_unref(op);
    
    m_isMuted = !m_isMuted;
}

float AudioManager::GetVolume() {
    return m_currentVolume;
}

void AudioManager::SuccessCallback(pa_context* c, int success, void* userdata) {
    if (!success) {
        std::cerr << "PulseAudio operation failed" << std::endl;
    }
}