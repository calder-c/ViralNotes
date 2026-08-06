cmake_minimum_required(VERSION 3.16)
project(MusicVisLib LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML 3
find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Window System)

add_library(midifile STATIC IMPORTED)
set_target_properties(midifile PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/midifile/lib/libmidifile.a"
)
# Main executable
add_executable(MusicVisLib
        src/ring.cpp
        src/disc.h
)


# Link everything
target_link_libraries(MusicVisLib PRIVATE
        SFML::Graphics
        SFML::Audio
        SFML::Window
        SFML::System
        midifile
)
