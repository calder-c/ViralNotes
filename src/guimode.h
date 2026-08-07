#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include <filesystem>
#include <random>
#include <algorithm>
#include <complex>
#include <thread>
#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"
#include "../tfd/tinyfiledialogs.h"
#include "easing.h"
#include "../nlohmann/json.hpp"
#ifndef MUSICVISLIB_GUIMODE_H
#define MUSICVISLIB_GUIMODE_H

#endif //MUSICVISLIB_GUIMODE_H
using namespace nlohmann;
struct VisualizerSettings {
    std::string midiFilename, musicFilename;
    float preDelay = 1;
    float playOffset = 4;
    float circleRadius = 200;
    float outlineThickness = 2;
    int circlePointCount = 1200;
    float hueCyclingFactor = 7.0f;
    float hueOffset = 300.0f;
    float BPM = 120;
    unsigned int SCREEN_X = 800, SCREEN_Y = 600;
    int maxThreads = 200;
    std::string exportPath = "./output.mp4";
    bool doSave = false;
    std::string funcIn = "linear";
    std::string funcOut = "linear";
    int currentItemIn = 0;
    int currentItemOut = 0;
};

inline void to_json(json& j,  const VisualizerSettings& s) {
    j = json{
        {"midiFilename", s.midiFilename},
        {"musicFilename", s.musicFilename},
        {"preDelay", s.preDelay},
        {"playOffset", s.playOffset},
        {"circleRadius", s.circleRadius},
        {"circlePointCount", s.circlePointCount},
        {"outlineThickness", s.outlineThickness},
        {"hueCyclingFactor", s.hueCyclingFactor},
        {"hueOffset", s.hueOffset},
        {"BPM", s.BPM},
        {"screenX", s.SCREEN_X},
        {"screenY", s.SCREEN_Y},
        {"maxThreads", s.maxThreads},
        {"exportPath", s.exportPath},
        {"doSave", s.doSave},
        {"funcIn", s.funcIn},
        {"funcOut", s.funcOut},
        {"currentItemIn", s.currentItemIn},
        {"currentItemOut", s.currentItemOut},
    };
}

inline void from_json(const json& j, VisualizerSettings& s)
{
    s.midiFilename       = j.value("midiFilename", s.midiFilename);
    s.musicFilename      = j.value("musicFilename", s.musicFilename);
    s.preDelay           = j.value("preDelay", s.preDelay);
    s.playOffset         = j.value("playOffset", s.playOffset);
    s.circleRadius       = j.value("circleRadius", s.circleRadius);
    s.circlePointCount   = j.value("circlePointCount", s.circlePointCount);
    s.outlineThickness   = j.value("outlineThickness", s.outlineThickness);
    s.hueCyclingFactor   = j.value("hueCyclingFactor", s.hueCyclingFactor);
    s.hueOffset          = j.value("hueOffset", s.hueOffset);
    s.BPM                = j.value("BPM", s.BPM);
    s.SCREEN_X           = j.value("screenX", s.SCREEN_X);
    s.SCREEN_Y           = j.value("screenY", s.SCREEN_Y);
    s.maxThreads         = j.value("maxThreads", s.maxThreads);
    s.exportPath         = j.value("exportPath", s.exportPath);
    s.doSave             = j.value("doSave", s.doSave);
    s.funcIn             = j.value("funcIn", s.funcIn);
    s.funcOut            = j.value("funcOut", s.funcOut);
    s.currentItemIn      = j.value("currentItemIn", s.currentItemIn);
    s.currentItemOut     = j.value("currentItemOut", s.currentItemOut);
}

void doImguiLoop(sf::Vector2u guiDimensions, VisualizerSettings & settings);