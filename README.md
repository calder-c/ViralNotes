
# Viral Notes

A C++ project aiming to recreate those viral note videos online that gain millions of views using SFML.

## Introduction
The initial idea was based on [this reel](https://www.instagram.com/reels/DbjUjOwt7d4/) that I found on Instagram from @bitdocmusic. I noticed that it was a fairly simple concept: little spheres flying out and seeming to trigger notes upon hitting the edge of the circle that contained them. I wondered if I could recreate this in C++ open source for anyone to generate their own videos and potentially go viral!

## Demo
https://github.com/user-attachments/assets/b2476afc-0c07-4d11-9d9b-4fccbbd65069

## Dependencies
 - [cmake version 3.5+](https://cmake.org/download/)
 - C++ standard 20+
 - ffmpeg added to PATH (*optional if not recording*)


Automatically build through CMakeLists:
- SFML 3.x
- [midifile](https://github.com/craigsapp/midifile)
- [nlohmann/json](https://github.com/nlohmann/json)
- [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/)
- [imgui-SFML](https://github.com/SFML/imgui-sfml)

## Installation and Usage
Clone the repo 

`git clone https://github.com/calder-c/ViralNotes.git`


### Building from source

`cmake -S ./ -B build && cd ./build/ && make`

or without make:

`cmake -S ./ -B build && cmake --build build`

___
simply supply the `--gui` flag to open an ImGui window to set and save all parameters


___
## Other Notes

Make sure that the midi files and the song files are perfectly in sync for the bpm.
I have used Ableton to do this and I hope to provide some example files here soon.

Supported song formats:
`wav` `mp3` `ogg` `flac`

**Known issue:** some midi formats are seemingly not able to be parsed by midifile, but most (especially ones exported by Ableton) seem to be fine. Hopefully will be fixed in next major version.