#pragma once
#include "entity/Background.h"
#include "entity/Ground.h"
#include "states/State.h"
#include <SFML/Graphics.hpp>

class TitleScreenState: public State {
private:
    int m_width, m_height;
    float m_groundHeight = 128.0f;
    float m_backgroundHeight = 472.0f;

    sf::Text m_title;
    Ground m_ground;
    Background m_background;

    float m_timeElapsed = 0.f;

public:
    TitleScreenState(int width, int height, StateStackInterface& stateStack);
    ~TitleScreenState();

    void handleEvent(sf::Event event);
    void update(float elapsed);
    void draw(sf::RenderWindow& window) const;
};