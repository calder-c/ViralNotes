cmake_minimum_required(VERSION 3.5)
project(MusicVisLib LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Override minimum policy version")
set(SFML_STATIC_LIBRARIES OFF)
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
# SFML 3
include(FetchContent)
FetchContent_Declare(
        reproc++
        GIT_REPOSITORY https://github.com/daandemeyer/reproc.git
        GIT_TAG        06034a7fca1ec46eddb4997f7764db89380c5216
)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        8835b6b955c96fa1ad1278a8dae6f05c4143effc
)
FetchContent_Declare(
        midifile
        GIT_REPOSITORY https://github.com/craigsapp/midifile.git
        GIT_TAG        master
)
FetchContent_Declare(
        tfd
        GIT_REPOSITORY https://github.com/native-toolkit/libtinyfiledialogs.git
        GIT_TAG        master
)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        55f93686c01528224f448c19128836e7df245f72
)
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        f5befd2d29e66809cd1110a152e375a7f1981f06
)
FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG        5e8f8fb857c8f3cff1096bf3e4402c64e3bcf6af
)
set(REPROC++ ON CACHE BOOL "Build reproc++ API wrapper" FORCE)
FetchContent_MakeAvailable(reproc++ SFML midifile tfd nlohmann_json imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
FetchContent_MakeAvailable(imgui-sfml)

find_package(OpenGL REQUIRED)

# Midifile

#add_library(midifile STATIC
#    midifile/src/Binasc.cpp
#    midifile/src/MidiEvent.cpp
#    midifile/src/MidiEventList.cpp
#    midifile/src/MidiFile.cpp
#    midifile/src/MidiMessage.cpp
#    midifile/src/Options.cpp
#)

#target_include_directories(midifile PUBLIC
#    midifile/include
#)

# Main executable

add_executable(MusicVisLib
    src/ring.cpp
    src/guimode.cpp
    src/guimode.h
    src/easing.h
        src/fonts.h
)


add_library(tfd STATIC
        ${tfd_SOURCE_DIR}/tinyfiledialogs.c
)

target_include_directories(tfd PUBLIC
        ${tfd_SOURCE_DIR}
)
target_include_directories(midifile PUBLIC
        ${midifile_SOURCE_DIR}/include
)
#target_include_directories(reproc++ PUBLIC
#        ${reproc++_SOURCE_DIR}/include/reproc++
#)

if(TARGET OpenGL::OpenGL)
    set(OPENGL_TARGET OpenGL::OpenGL)
else()
    set(OPENGL_TARGET OpenGL::GL)
endif()

target_link_libraries(MusicVisLib PRIVATE
        ${OPENGL_TARGET}
        SFML::Graphics
        SFML::Audio
        SFML::Window
        SFML::System
        reproc++
        midifile
        tfd
        nlohmann_json::nlohmann_json
        ImGui-SFML::ImGui-SFML

)

if (APPLE)
    target_link_libraries(MusicVisLib PRIVATE
        "-framework Cocoa"
    )
endif()
