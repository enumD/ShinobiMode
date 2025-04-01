cmake_minimum_required(VERSION 3.10)
project(my_app)

set(CMAKE_CXX_STANDARD 17)

# 1. ImGui come submodule
add_subdirectory(extern/imgui)

# 2. Trova le altre dipendenze
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(nlohmann_json 3.0 REQUIRED)

# 3. Configura backend ImGui (GLFW+OpenGL3)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui)
add_library(imgui_impl STATIC
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_impl PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
target_link_libraries(imgui_impl PRIVATE glfw OpenGL::GL)

# 4. Configura il tuo eseguibile
add_executable(my_app
    main.cpp
    gui/Toolbar.cpp
    gui/OptionsScreen.cpp
    utils/ConfigManager.cpp
)

# 5. Collegamento librerie
target_link_libraries(my_app PRIVATE
    glfw
    OpenGL::GL
    imgui
    imgui_impl
    nlohmann_json::nlohmann_json
)