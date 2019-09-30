#include "states/ModeSelectState.h"
#include "states/PlayState.h"
#include "states/TrainState.h"
#include "states/CPUPlayState.h"
#include "states/FadeState.h"
#include "asset/AssetManager.h"
#include <cmath>
#include <iostream>

ModeSelectState::ModeSelectState(int width, int height, StateStackInterface& stateStack)
    : State(stateStack), m_width(width), m_height(height),
      m_ground(sf::FloatRect(0.0f, height - m_groundHeight, width, m_groundHeight)),
      m_background(sf::FloatRect(0.0f, 0.0f, width, m_backgroundHeight)) {

    // Setup score text
    sf::Font &font = AssetManager::instance().getFont("data/flappy.ttf");
    m_text.setFont(font);
    m_text.setString("Select mode");
    m_text.setCharacterSize(80);
    m_text.setFillColor(sf::Color(255, 255, 255));
    m_text.setOutlineThickness(1.0f);
    m_text.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = m_text.getLocalBounds();

    m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    m_text.setPosition(sf::Vector2f(width / 2, textRect.height * 1.5f));

    // Setup buttons
    float paddingLeft = 10.f; // TODO: padding + dimensions should be within screen bounds.
    float paddingTop = textRect.height * 2.5f;
    float buttonSpacing = 5.f;
    float buttonWidth = 250.f;
    float buttonHeight = 50.f;
    unsigned int characterSize = 40;
    StatefulColorsDef buttonColors = {sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent};
    StatefulColorsDef textColors = {sf::Color::White, sf::Color::White, sf::Color::Red};
    StatefulColorsDef outlineColors = {sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent};
    
    m_buttons[0].initialize(paddingLeft, paddingTop, 
                            buttonWidth, buttonHeight, "Play mode", font, characterSize,
                            [this](){
                                // TODO: The fact that I need this "replaceTop" function means that this is the wrong pattern to use.
                                // TODO: Let's manage all memory correctly
                                this->m_stateStack.replaceTop(std::make_shared<PlayState>(this->m_width, this->m_height, this->m_stateStack));
                            },
                            textColors, buttonColors, outlineColors);

    m_buttons[1].initialize(paddingLeft, paddingTop + buttonHeight + buttonSpacing, 
                            buttonWidth, buttonHeight, "Train mode", font, characterSize,
                            [this](){
                                this->m_stateStack.replaceTop(std::make_shared<TrainState>(this->m_width, this->m_height, this->m_stateStack));
                            },
                            textColors, buttonColors, outlineColors);

    m_buttons[2].initialize(paddingLeft, paddingTop + 2 * (buttonHeight + buttonSpacing), 
                            buttonWidth, buttonHeight, "CPU mode", font, characterSize,
                            [this](){
                                this->m_stateStack.replaceTop(std::make_shared<CPUPlayState>(this->m_width, this->m_height, this->m_stateStack));
                            },
                            textColors, buttonColors, outlineColors);
}

ModeSelectState::~ModeSelectState() {}

void ModeSelectState::handleEvent(sf::Event event) {
    for (auto& button : m_buttons) {
        button.handleEvent(event);
    }
}

void ModeSelectState::update(float elapsed) {
    for (auto& button : m_buttons) {
        button.update(elapsed);
    }
}

void ModeSelectState::draw(sf::RenderWindow& window) const {
    m_background.draw(window);
    m_ground.draw(window);
    window.draw(m_text);

    for (auto& button : m_buttons) {
        button.draw(window);
    }
}