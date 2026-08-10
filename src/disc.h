#include <SFML/Graphics.hpp>
#include "easing.h"
class Disc {

public:
    sf::CircleShape shape;
    sf::Vector2f pos, destination, startPos;
    float travelSeconds;
    bool hasReachedDestination, doRender;
    int timesReached;
    int noteNumber;
    std::string funcIn, funcOut;
    float timeElapsed = 0.0;

    sf::Vector2f stepPerSecond;
    Disc(sf::Vector2f pos_, sf::Vector2f destination_, float travelSeconds_, sf::Color color = sf::Color::Red, int noteNumber_ = 0, std::string funcIn_ = "linear", std::string funcOut_ = "linear") {
        funcIn = funcIn_;
        funcOut = funcOut_;
        sf::Color mutedColor = color;
        noteNumber = noteNumber_;
        mutedColor.r /= 2;
        mutedColor.g /= 2;
        mutedColor.b /= 2;
        hasReachedDestination = false;
        doRender = true;
        timesReached = 0;
        pos = pos_;
        startPos = pos;
        destination = destination_;
        travelSeconds = travelSeconds_;
        shape = sf::CircleShape();
        shape.setOutlineColor(color);
        shape.setFillColor(mutedColor);
        shape.setRadius(10);
        shape.setOutlineThickness(-5);
        shape.setOrigin({10.f, 10.f});
        shape.setPosition(pos);
        stepPerSecond = (destination-pos)/travelSeconds;
    }


    int update(float dt) {
        if (hasReachedDestination) {
            return timesReached;
        }
        pos+=stepPerSecond*dt;
        timeElapsed += dt;
        float t = timeElapsed / travelSeconds;

        if (t >= 1.f) {
            pos = destination;
            startPos = pos;
            hasReachedDestination = true;
            timesReached +=1;
        } else {

            if (timesReached < 1) {
                pos = startPos + (destination - startPos) * selectEasing(t, funcIn);
            } else {
                pos = startPos + (destination - startPos) * selectEasing(t, funcOut);
            }

        }
        shape.setPosition(pos);

        return 0;
    }
    void setNewDestination(sf::Vector2f destination_, float travelSeconds_) {
        hasReachedDestination = false;
        destination = destination_;
        travelSeconds = travelSeconds_;
        stepPerSecond = (destination-pos)/travelSeconds;
        timeElapsed = 0.0;
    }
    void render(sf::RenderWindow & window) {
        window.draw(shape);

    }
};