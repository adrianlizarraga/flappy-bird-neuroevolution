#pragma once
#include "entity/Background.h"
#include "entity/Ground.h"
#include "states/State.h"
#include "gui/Button.h"
#include <SFML/Graphics.hpp>

class ModeSelectState: public State {
private:
    int m_width, m_height;
    float m_groundHeight = 128.0f;
    float m_backgroundHeight = 472.0f;

    sf::Text m_text;
    Ground m_ground;
    Background m_background;

    Button m_buttons[3];

    float m_timeElapsed = 0.f;

public:
    ModeSelectState(int width, int height, StateStackInterface& stateStack);
    ~ModeSelectState();

    void handleEvent(sf::Event event);
    void update(float elapsed);
    void draw(sf::RenderWindow& window) const;
};