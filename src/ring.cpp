#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include <filesystem>
#include <random>
#include <algorithm>
#include <complex>
#include <thread>

#include "midi.h"
#include "disc.h"


sf::Color getRainbowColor(float hue) {
    // Wrap hue around if it exceeds 360
    hue = std::fmod(hue, 360.0f);
    if (hue < 0) hue += 360.0f;

    float x = (1.0f - std::abs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
    float r = 0, g = 0, b = 0;

    if (hue < 60)       { r = 1; g = x; b = 0; }
    else if (hue < 120) { r = x; g = 1; b = 0; }
    else if (hue < 180) { r = 0; g = 1; b = x; }
    else if (hue < 240) { r = 0; g = x; b = 1; }
    else if (hue < 300) { r = x; g = 0; b = 1; }
    else                { r = 1; g = 0; b = x; }

    return sf::Color(
        r * 255,
        g * 255,
        b*255
    );
}
//AI method here to get circle points because I suck at math
std::vector<sf::Vector2f> getCirclePoints(double radius, int numberOfPoints, float centerX = 0.0, float centerY = 0.0) {
    std::vector<sf::Vector2f> points;
    points.reserve(numberOfPoints);


    const float pi = 3.14159265358979323846;

    for (int i = 0; i < numberOfPoints; ++i) {
        // Calculate the angle for the current point in radians
        float angle = (2.0 * pi * i) / numberOfPoints;

        sf::Vector2f p;
        p.x = centerX + radius * std::cos(angle);
        p.y = centerY + radius * std::sin(angle);

        points.push_back(p);
    }

    return points;
}

int main(int argc, char** argv) {
    std::string midiFilename, musicFilename;
    float BPM;
    unsigned int SCREEN_X = 800, SCREEN_Y = 600;
    bool setMidi, setSound, setBPM, doSave = false;
    std::string arg;
    srand(time(NULL));
    if (argc == 1) {
        std::cout << "No args... terminating";
        return -1;
    }
    for (int i = 0; i < argc; ++i) {
        arg = argv[i];
        if (arg == "--midi" || arg == "-mid" || arg == "-m") {
            midiFilename = argv[++i];
            setMidi = true;
        }
        else if (arg == "--sound" || arg == "-s") {
            musicFilename = argv[++i];
            setSound = true;
        }
        else if (arg == "--BPM" || arg == "--tempo") {
            BPM = std::stof(argv[++i]);
            setBPM = true;
        }
        else if (arg == "--size") {
            SCREEN_X = std::stoi(argv[++i]);
            SCREEN_Y = std::stoi(argv[++i]);
        }
        else if (arg == "--export" || arg == "--save") {
            doSave = true;
        }
    }
    std::cout << "Recieved arguments: \nMIDI - " << midiFilename << "\nSound - " << musicFilename << "\nBPM - " << BPM << "\n";
    if (setMidi && setSound && setBPM) {
        std::cout << "Correctly caught args... initializing window\n";
    } else {
        throw std::invalid_argument("Caught bad args, terminating.");
    }
    musicFilename = std::filesystem::canonical(musicFilename);
    midiFilename = std::filesystem::canonical(midiFilename);

    // std::ifstream file(midiFilename, std::ios::binary);
    //
    // std::cout << std::boolalpha
    //           << "Open: " << file.is_open() << '\n';
    sf::ContextSettings windowSettings;
    windowSettings.antiAliasingLevel = 8;
    sf::RenderWindow window{sf::VideoMode({SCREEN_X, SCREEN_Y}), "Ring Visualiser", sf::Style::Default, sf::State::Windowed, windowSettings};
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(false);
    MidiData midiData(midiFilename, BPM);
    sf::SoundBuffer musicBuf(musicFilename);
    sf::Sound music(musicBuf);
    sf::Clock dtClock, elapsedTimeClock;
    sf::Font globalFont("../font/cour.ttf");

    float elapsedTime = 0;
    float preDelay = 2;
    float playOffset = 4;

    float circleCenterX = SCREEN_X/2.0f,
    circleCenterY = SCREEN_Y/2.0f,
    circleRadius = 200.0f,
    outlineThickness = 2,
    currentOutlineThickness = 2;
    bool musicPlaying = false;
    std::vector<Disc*> discList;
    std::vector<int> noteNumbers;
    // text obj, the white effect offset
    std::vector<std::pair<sf::Text*, float>> drawableTextList;
    std::map<int, sf::Vector2f> noteToPos;
    std::map<int, sf::Color> noteToColor;
    std::vector<sf::Vector2f> circlePoints;
    sf::CircleShape circle;
    circle.setPointCount(1200);
    for (auto & note : midiData.notes) {
        note->absNoteOnTime += playOffset;
        note->absNoteOffTime += playOffset;
        if(std::find(noteNumbers.begin(), noteNumbers.end(), note->noteNumber) != noteNumbers.end()) {
            //do nothing
        } else {
            noteNumbers.push_back(note->noteNumber);
        }
    }
    int index = 0;
    std::sort(noteNumbers.begin(), noteNumbers.end());
    for (auto & noteNum : noteNumbers) {
        noteToColor[noteNum] = getRainbowColor(index*30.0f);
        index++;

    }
    circlePoints = getCirclePoints(circleRadius, noteNumbers.size(), circleCenterX, circleCenterY);
    circle.setOutlineColor(sf::Color::White);
    circle.setFillColor(sf::Color::Black);
    circle.setRadius(circleRadius);
    circle.setOrigin(sf::Vector2f{circleRadius, circleRadius});
    circle.setOutlineThickness(outlineThickness);
    circle.setPosition(sf::Vector2f{circleCenterX, circleCenterY});
    index = 0;
    for (auto noteNum : noteNumbers) {
        noteToPos[noteNum] = circlePoints[index];
        sf::Text* newNumberText = new sf::Text(globalFont);
        newNumberText->setString(std::to_string(noteNum));
        newNumberText->setFillColor(noteToColor[noteNum]);
        newNumberText->setCharacterSize(24);
        //newNumberText->setStyle(sf::Text::Underlined | sf::Text::Italic);
        sf::Vector2f textPos = noteToPos[noteNum];
        textPos += (textPos - sf::Vector2f{circleCenterX, circleCenterY})/8.0f;
        newNumberText->setPosition(textPos);
        sf::FloatRect textBounds = newNumberText->getLocalBounds();
        newNumberText->setOrigin({
            textBounds.position.x + textBounds.size.x / 2.0f,
            textBounds.position.y + textBounds.size.y / 2.0f
        });
        drawableTextList.push_back(std::pair(newNumberText, 0));
        index++;
    }
    int frameCount = 0;
    float exportFPS = 60.0f;
    float fixedDt = 1.0f / exportFPS;
    std::string dirName;
    if (doSave) {
        dirName = "exported" + std::to_string(rand() % 1000);
        std::filesystem::create_directory("./" + dirName);
    }
    while (window.isOpen()){
        frameCount++;
        window.clear();
        if (doSave) {
            elapsedTime += fixedDt;
        } else {
            elapsedTime = elapsedTimeClock.getElapsedTime().asSeconds();
        }
        float dt;
        if (doSave) {
            dt = fixedDt;
        } else {
            sf::Time dtTime = dtClock.restart();
            dt = dtTime.asSeconds();
        }

        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        for (auto & note : midiData.notes) {
            if (elapsedTime >= (note->absNoteOnTime - preDelay) && !note->passed) {
                note->passed = true;
                discList.push_back(new Disc(sf::Vector2f{circleCenterX, circleCenterY}, noteToPos[note->noteNumber], preDelay, noteToColor[note->noteNumber], note->noteNumber));

            }
        }

        if (elapsedTime >= playOffset && !doSave) {
            if (!musicPlaying) {
                std::cout << "Started playback\n";
                music.play();
                musicPlaying = true;
            }

        }
        circle.setOutlineThickness(currentOutlineThickness);
        window.draw(circle);
        if (currentOutlineThickness <= outlineThickness) {
            currentOutlineThickness = outlineThickness;

        } else {
            currentOutlineThickness -= 20*dt;
        }


        for (auto & text : drawableTextList) {
            // 1. Get the base color for this specific note
            int currentNoteNum = std::stoi(text.first->getString().toAnsiString());
            sf::Color baseColor = noteToColor[currentNoteNum];

            if (text.second <= 0.0f) {
                text.second = 0.0f;
                text.first->setFillColor(baseColor);
                text.first->setStyle(sf::Text::Regular);

            } else {
                text.second -= 1500.0f * dt;

                float factor = std::clamp(text.second / 500.0f, 0.0f, 1.0f);

                sf::Color flashedColor(
                    baseColor.r + (255 - baseColor.r) * factor,
                    baseColor.g + (255 - baseColor.g) * factor,
                    baseColor.b + (255 - baseColor.b) * factor
                );

                text.first->setFillColor(flashedColor);
            }

            window.draw(*text.first);
        }
        for (auto & disc : discList) {
            int result = disc->update(dt);
            disc->render(window);
            if (result == 1) {
                disc->setNewDestination(sf::Vector2f(circleCenterX, circleCenterY), preDelay);
                disc->shape.setOutlineColor(disc->shape.getFillColor());
                sf::Color mutedColor = disc->shape.getOutlineColor();
                mutedColor.r /= 3;
                mutedColor.g /= 3;
                mutedColor.b /= 3;
                disc->shape.setFillColor(mutedColor);
                currentOutlineThickness = outlineThickness + 10;
                circle.setOutlineColor(disc->shape.getOutlineColor());
                for (auto & text : drawableTextList) {
                    if (std::stoi(text.first->getString().toAnsiString()) == disc->noteNumber) {
                        text.first->setStyle(sf::Text::Bold);
                        text.second = 500.0f;
                    }
                }
            } else if (result == 2) {
                disc->doRender = false;
            }
        }
        window.display();
        if (doSave) {
            sf::Vector2u size = window.getSize();
            sf::Texture texture;
            texture.resize(sf::Vector2u{size.x, size.y});
            texture.update(window);
            sf::Image screenshot = texture.copyToImage();
            std::string filepath = "./" + dirName + "/" + std::to_string(frameCount) + ".png";
            std::thread([screenshot = std::move(screenshot), filepath]() {
                screenshot.saveToFile(filepath);
            }).detach();


        }
    }
    system(("cd ./" + dirName + " && ""ffmpeg -framerate 60 -f image2 -pattern_type sequence -start_number 0 -i '%d.png' -itsoffset " + std::to_string(playOffset) + " -i " + musicFilename + " -vf \"scale=trunc(iw/2)*2:trunc(ih/2)*2\" -pix_fmt yuv420p output.mp4").c_str());
}