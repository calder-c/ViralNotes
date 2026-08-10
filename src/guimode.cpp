#include "guimode.h"
#include "presets.h"
#include <fstream>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
void saveSettings(const std::string& filename, const VisualizerSettings &settings) {
    std::ofstream o(filename);
    json j_out = settings;
    o << std::setw(4) << j_out << std::endl;
}

VisualizerSettings loadSettings(const std::string& filename) {
    std::ifstream i(filename);
    json j_in;
    i >> j_in;
    VisualizerSettings settings;
    j_in.get_to(settings);
    return settings;
}

void doImguiLoop(sf::Vector2u guiDimensions, VisualizerSettings & settings) {
    sf::RenderWindow window{sf::VideoMode({guiDimensions.x, guiDimensions.y}), "Visualiser GUI", sf::Style::Default, sf::State::Windowed};
    window.setFramerateLimit(30);
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>()){
                window.close();
                exit(1);
            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
        {
            window.close();
        }
        static bool advancedSettings = false;
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::SetNextWindowSize(ImVec2{static_cast<float>(guiDimensions.x), static_cast<float>(guiDimensions.y)});
        ImGui::SetNextWindowPos(ImVec2{0, 0});
        ImGui::Begin("Visualizer Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Window Settings");
        ImGui::Text("FX Settings");
        ImGui::Checkbox("Recording Only", &settings.doSave);
        static int currentVideoPreset = 2;
        settings.selectedVideoPreset = currentVideoPreset;
        if (settings.doSave) {
            const char * videoFormats[1] = {"*.mp4"};
            if (ImGui::Button("Choose export path (MP4)")) {
                if (const char *fileChosen = tinyfd_saveFileDialog("Choose export mp4 path", settings.exportPath.c_str(), 1, videoFormats,"MP4 Files"); fileChosen != nullptr) {
                    settings.exportPath = fileChosen;
                }
            }
            ImGui::ListBox("Quality Preset", &currentVideoPreset, allowedPresets, IM_ARRAYSIZE(allowedPresets), 1);
        }
        ImGui::Checkbox("Advanced Settings", &advancedSettings);
        ImGui::SliderInt("Center Circle Point Count", &settings.circlePointCount, 0.0f, 1200.0f);
        ImGui::SliderFloat("Center Circle Outline Thickness", &settings.outlineThickness, 1.0f, 100.0f);
        ImGui::SliderFloat("Center Circle Radius", &settings.circleRadius, 1.0f, 1000.0f);
        ImGui::SliderFloat("Hue Cycling Factor", &settings.hueCyclingFactor, 0.0f, 365.0f);
        ImGui::SliderFloat("Starting Hue", &settings.hueOffset, 0.0f, 365.0f);
        ImVec4 rgb_color;
        ImGui::ColorConvertHSVtoRGB(settings.hueOffset/365.0f, 1, 1, rgb_color.x, rgb_color.y, rgb_color.z);
        rgb_color.w = 1.0;
        ImGui::ColorButton("##ColorDisplay", rgb_color, ImGuiColorEditFlags_None, ImVec2(50, 50));
        ImGui::Text("Note Settings");
        ImGui::InputFloat("BPM", &settings.BPM, 1.f, 50.0f);
        ImGui::SliderFloat("Note Travel Time", &settings.preDelay, 0.f, 20.0f);
        ImGui::ListBox("Note Travel Function (In)", &settings.currentItemIn, easings, IM_ARRAYSIZE(easings), 1);
        settings.funcIn = easings[settings.currentItemIn];
        ImGui::ListBox("Note Travel Function (Out)", &settings.currentItemOut, easings, IM_ARRAYSIZE(easings), 1);
        settings.funcOut = easings[settings.currentItemOut];
        ImGui::SliderFloat("Music Play Offset", &settings.playOffset, 1.f, 30.0f);
        if (settings.playOffset <= settings.preDelay) {
            settings.playOffset = settings.preDelay+1;
        }

        const char* midiFormats[2] = {"*.mid", "*.midi"};
        const char* musicFormats[4] = {"*.mp3", "*.ogg", "*.wav", "*.flac"};
        if (ImGui::Button("Choose MIDI File")) {
            if (const char *fileChosen = tinyfd_openFileDialog("Choose MIDI file", settings.midiFilename.c_str(), 2, midiFormats,"MIDI Files", 0); fileChosen != nullptr) {
                settings.midiFilename = fileChosen;
            }
        }
        ImGui::Text(("MIDI File:" + settings.midiFilename).c_str());
        if (ImGui::Button("Choose Music File")) {
            if (const char *fileChosen = tinyfd_openFileDialog("Choose Music File", settings.musicFilename.c_str(), 4, musicFormats,"Music Files", 0); fileChosen != nullptr) {
                settings.musicFilename = fileChosen;
            }
        }
        ImGui::Text(("Music File:" + settings.musicFilename).c_str());
        if (ImGui::Button("Save to .json")) {
            const char* jsonFormats[1] = {"*.json"};
            if (const char *fileChosen = tinyfd_saveFileDialog("Choose save path", "settings.json", 0, jsonFormats,"JSON Files"); fileChosen != nullptr) {
                saveSettings(fileChosen, settings);
            }
        }
        if (ImGui::Button("Load from .json")) {
            char const* jsonFormats[] = {"*.json"};
            if (const char *fileChosen = tinyfd_openFileDialog("Choose load path", "settings.json", 0, jsonFormats,"JSON Files", 0); fileChosen != nullptr) {
                VisualizerSettings newSettings = loadSettings(fileChosen);
                settings = newSettings;
            }
        }
        if (advancedSettings) {
            ImGui::Text("MIDI File Absolute Path");
            ImGui::InputText("##MIDI File Absolute Path", &settings.midiFilename, 0, nullptr, &settings.midiFilename);
            ImGui::Text("Music File Absolute Path");
            ImGui::InputText("##Music File Absolute Path", &settings.musicFilename, 0, nullptr, &settings.musicFilename);

        }
        if (ImGui::Button("Start Playback [ENTER]")) {
            window.close();
        }

        ImGui::End();
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

}