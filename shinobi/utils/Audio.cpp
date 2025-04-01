#include "Audio.h"
#include <pulse/pulseaudio.h>
#include <iostream>
#include <fstream>
Audio::Audio() {}

Audio::~Audio() {}



void Audio::setVolume(int volume) {
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;

    // Create mainloop and context
    pa_mainloop *mainloop = pa_mainloop_new();
    pa_context *context = pa_context_new(pa_mainloop_get_api(mainloop), "VolumeControl");

    // Connect to PulseAudio context
    if (pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr) < 0) {
        std::cerr << "Error: Unable to connect to PulseAudio!" << std::endl;
        pa_mainloop_free(mainloop);
        return;
    }

    // Wait until the context is ready
    while (pa_context_get_state(context) != PA_CONTEXT_READY) {
        pa_mainloop_iterate(mainloop, 1, nullptr);
    }

    // Get the server info and check default sink
    const pa_server_info *server_info = nullptr;
    pa_context_get_server_info(context, [](pa_context *, const pa_server_info *info, void *userdata) {
        *reinterpret_cast<const pa_server_info **>(userdata) = info;
    }, &server_info);

    // Loop to ensure we get the info
    while (!server_info) {
        pa_mainloop_iterate(mainloop, 1, nullptr);
    }

    if (!server_info || !server_info->default_sink_name) {
        std::cerr << "Error: No default audio sink found!" << std::endl;
        pa_context_disconnect(context);
        pa_context_unref(context);
        pa_mainloop_free(mainloop);
        return;
    }

    std::cout << "Default Sink: " << server_info->default_sink_name << std::endl;

    // Convert volume (0-100%) to PulseAudio scale (0-65536)
    pa_cvolume cvol;
    pa_cvolume_set(&cvol, 1, volume * PA_VOLUME_NORM / 100);

    // Set volume
    pa_operation *op = pa_context_set_sink_volume_by_name(context, server_info->default_sink_name, &cvol, nullptr, nullptr);
    if (op) {
        // Wait for operation to complete
        while (pa_operation_get_state(op) == PA_OPERATION_RUNNING) {
            pa_mainloop_iterate(mainloop, 1, nullptr);
        }
        pa_operation_unref(op);
    } else {
        std::cerr << "Error: Failed to set volume!" << std::endl;
    }

    // Disconnect and clean up
    pa_context_disconnect(context);
    pa_context_unref(context);
    pa_mainloop_free(mainloop);
}


// void Audio::setVolume(int volume) {
//     if (volume < 0) volume = 0;
//     if (volume > 100) volume = 100;

//     pa_mainloop *mainloop = pa_mainloop_new();
//     pa_context *context = pa_context_new(pa_mainloop_get_api(mainloop), "VolumeControl");

//     pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
//     pa_mainloop_iterate(mainloop, 1, nullptr);

//     while (pa_context_get_state(context) != PA_CONTEXT_READY) {
//         pa_mainloop_iterate(mainloop, 1, nullptr);
//     }

//     // Get default sink name
//     const pa_server_info *server_info;
//     pa_context_get_server_info(context, [](pa_context *, const pa_server_info *info, void *userdata) {
//         *reinterpret_cast<const pa_server_info **>(userdata) = info;
//     }, &server_info);
//     pa_mainloop_iterate(mainloop, 1, nullptr);

//     if (!server_info || !server_info->default_sink_name) {
//         std::cerr << "Error: No default audio sink found!" << std::endl;
//         pa_context_disconnect(context);
//         pa_context_unref(context);
//         pa_mainloop_free(mainloop);
//         return;
//     }

//     // Convert volume (0-100%) to PulseAudio scale (0-65536)
//     pa_cvolume cvol;
//     pa_cvolume_set(&cvol, 1, volume * PA_VOLUME_NORM / 100);

//     // Set volume
//     pa_operation *op = pa_context_set_sink_volume_by_name(context,server_info->default_sink_name, &cvol, nullptr, nullptr);
//     if (op) pa_operation_unref(op);

//     pa_context_disconnect(context);
//     pa_context_unref(context);
//     pa_mainloop_free(mainloop);
// }




// void Audio::setVolume(int volume) {
//     if (volume < 0) volume = 0;
//     if (volume > 100) volume = 100;

//     pa_mainloop *mainloop = pa_mainloop_new();
//     pa_context *context = pa_context_new(pa_mainloop_get_api(mainloop), "VolumeControl");

//     pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
//     pa_mainloop_iterate(mainloop, 1, nullptr);

//     // Wait for the context to be ready
//     while (pa_context_get_state(context) != PA_CONTEXT_READY) {
//         pa_mainloop_iterate(mainloop, 1, nullptr);
//     }

//     // Convert volume percentage (0-100) to PulseAudio scale (0-65536)
//     pa_cvolume cvol;
//     pa_cvolume_set(&cvol, 1, volume * PA_VOLUME_NORM / 100);

//     // Set the volume for the default audio sink
//     pa_operation *op = pa_context_set_sink_volume_by_index(context, 0, &cvol, nullptr, nullptr);
//     if (op != nullptr) {
//         pa_operation_unref(op);
//     }
//     // Clean up
//     pa_context_disconnect(context);
//     pa_context_unref(context);
//     pa_mainloop_free(mainloop);
// }
