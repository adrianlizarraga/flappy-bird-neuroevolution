#pragma once
#include "states/State.h"
#include <SFML/Graphics.hpp>
#include <functional>

class FadeState: public State {
private:
    int m_width, m_height;
    sf::RectangleShape m_rectangle;

    sf::Color m_fadeColor;
    int m_initOpacity;
    int m_finalOpacity;
    float m_fadeDuration;
    std::function<void()> m_fadeCompleteCallback;

    float m_timeElapsed = 0.f;

public:
    FadeState(int width, int height, StateStackInterface& stateStack, 
              sf::Color fadeColor, int initOpacity, int finalOpacity, float fadeDuration, 
              std::function<void()> fadeCompleteCallback);
    ~FadeState();

    void handleEvent(sf::Event event);
    void update(float elapsed);
    void draw(sf::RenderWindow& window) const;
};