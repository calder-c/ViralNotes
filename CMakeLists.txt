cmake_minimum_required(VERSION 3.16)
project(MusicVisLib LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML 3
find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Window System)
find_package(OpenGL REQUIRED)
add_library(midifile STATIC IMPORTED)
set_target_properties(midifile PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/midifile/lib/libmidifile.a"
)
# Main executable
add_executable(MusicVisLib
        src/ring.cpp
        src/guimode.cpp
        src/guimode.h
        src/easing.h
)

target_sources(MusicVisLib PRIVATE
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp
        imgui/imgui-SFML.cpp)

add_library(tfd tfd/tinyfiledialogs.c)

target_include_directories(tfd PUBLIC
        src/tfd
)
# Link everything
target_link_libraries(MusicVisLib PRIVATE
        OpenGL::GL
        SFML::Graphics
        SFML::Audio
        SFML::Window
        SFML::System
        midifile
        tfd

)
if (APPLE)
    target_link_libraries(MusicVisLib PRIVATE
            "-framework Cocoa"
    )
endif()