cmake_minimum_required(VERSION 3.10)
project(my_app)

set(CMAKE_CXX_STANDARD 17)

# Policy per evitare warning OpenGL
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

# 1. ImGui come submodule (con il TUO CMakeLists.txt in extern/imgui)
add_subdirectory(extern/imgui)

# 2. Trova le altre dipendenze
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(nlohmann_json 3.0 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PULSEAUDIO REQUIRED libpulse libpulse-simple)
pkg_check_modules(GLIB REQUIRED glib-2.0)


set(OpenCV_DIR /usr/lib/cmake/opencv4)  # <-- Controlla che il percorso sia corretto per il tuo sistema



# 3. Configura il tuo eseguibile
add_executable(my_app
    main.cpp
    gui/Toolbar.cpp
    gui/CameraStream.cpp
    gui/MyWindow.cpp
    gui/SettingWindow.cpp
    gui/CameraWindow.cpp
    gui/ModeSelector.cpp
    utils/AppSerializer.cpp
    utils/AppSetting.cpp
    utils/LayoutHelper.cpp
    utils/AppSerializer.cpp
    utils/Notification.cpp
    utils/Audio.cpp

)

# 4. Collegamento librerie
target_link_libraries(my_app PRIVATE
    glfw
    OpenGL::GL
    imgui                  # Da extern/imgui/CMakeLists.txt
    nlohmann_json::nlohmann_json
    ${OpenCV_LIBS}
    ${PULSEAUDIO_LIBRARIES}
    ${GLIB_LIBRARIES}
)

# 5. Include directories
target_include_directories(my_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends
    ${OpenCV_INCLUDE_DIRS}  # Fix corretto: aggiunto include di OpenCV
    ${PULSEAUDIO_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
)

target_compile_options(my_app PRIVATE
    ${PULSEAUDIO_CFLAGS_OTHER}
    ${GLIB_CFLAGS_OTHER}
)