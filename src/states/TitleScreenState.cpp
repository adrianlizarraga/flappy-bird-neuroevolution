#include "states/TitleScreenState.h"
#include "states/PlayState.h"
#include "asset/AssetManager.h"
#include <cmath>
#include <iostream>

TitleScreenState::TitleScreenState(int width, int height, StateStackInterface& stateStack)
    : State(stateStack), m_width(width), m_height(height),
      m_ground(sf::FloatRect(0.0f, height - m_groundHeight, width, m_groundHeight)),
      m_background(sf::FloatRect(0.0f, 0.0f, width, m_backgroundHeight)) {

    // Setup score text
    sf::Font &font = AssetManager::instance().getFont("data/flappy.ttf");
    m_title.setFont(font);
    m_title.setString("Press any key");
    m_title.setCharacterSize(80);
    m_title.setFillColor(sf::Color(255, 255, 255));
    m_title.setOutlineThickness(1.0f);
    m_title.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = m_title.getLocalBounds();

    m_title.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    m_title.setPosition(sf::Vector2f(width / 2, height / 2));
}

TitleScreenState::~TitleScreenState() {}

void TitleScreenState::handleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        m_stateStack.pop();
        m_stateStack.push(std::make_shared<PlayState>(m_width, m_height, m_stateStack));
    }
}

void TitleScreenState::update(float elapsed) {
    float minScale = 0.9f;
    float maxScale = 1.1f;
    float periodSec = 1.4f;
    float percent = m_timeElapsed / periodSec;

    // Pulse text (grow)
    if (percent <= 0.5f) {
        float scale = (1.f - percent * 2.f) * minScale + percent * 2.f * maxScale;

        m_title.setScale(scale, scale);
    }

    // Pulse text (shrink)
    else {
        float scale = (1.f - (percent - 0.5f) * 2.f) * maxScale + (percent - 0.5f) * 2.f * minScale;

        m_title.setScale(scale, scale);
    }

    // Only keep track of a period's worth of time...
    m_timeElapsed = std::fmod(m_timeElapsed + elapsed, periodSec);
}

void TitleScreenState::draw(sf::RenderWindow& window) const {
    m_background.draw(window);
    m_ground.draw(window);
    window.draw(m_title);
}