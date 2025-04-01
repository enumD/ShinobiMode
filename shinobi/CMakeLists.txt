cmake_minimum_required(VERSION 3.10)
project(my_app)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(nlohmann_json 3.0 REQUIRED)

add_subdirectory(imgui)

add_executable(my_app
    main.cpp
    gui/Toolbar.cpp
    gui/OptionsScreen.cpp
    utils/ConfigManager.cpp
)

target_link_libraries(my_app PRIVATE
    glfw
    OpenGL::GL
    imgui
    nlohmann_json::nlohmann_json
)