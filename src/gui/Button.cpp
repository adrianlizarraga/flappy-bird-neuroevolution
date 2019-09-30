#include "gui/Button.h"
#include <cmath>
#include <iostream>

Button::Button() {
    this->m_state = IDLE;
}

Button::Button(float x, float y, float width, float height, 
       const std::string& text, sf::Font& font, unsigned int characterSize,
       const StatefulColorsDef& textColors,
       const StatefulColorsDef& buttonColors,
       const StatefulColorsDef& outlineColors) : 
       Button(x, y, width, height, text, font, characterSize, [](){}, textColors, buttonColors, outlineColors) {

       }

Button::Button(float x, float y, float width, float height, 
       const std::string& text, sf::Font& font, unsigned int characterSize,
       std::function<void()> onPressedCallback,
       const StatefulColorsDef& textColors,
       const StatefulColorsDef& buttonColors,
       const StatefulColorsDef& outlineColors) {
           this->initialize(x, y, width, height, text, font, characterSize, 
                            onPressedCallback, textColors, buttonColors, outlineColors);
       }

void Button::initialize(float x, float y, float width, float height, 
       const std::string& text, sf::Font& font, unsigned int characterSize,
       std::function<void()> onPressedCallback,
       const StatefulColorsDef& textColors,
       const StatefulColorsDef& buttonColors,
       const StatefulColorsDef& outlineColors) {

    this->m_onPressedCallback = onPressedCallback;
    this->m_textColors = textColors;
    this->m_buttonColors = buttonColors;
    this->m_outlineColors = outlineColors;

    this->m_state = IDLE;

    // Setup shape
    this->m_shape.setSize(sf::Vector2f(width, height));
    this->m_shape.setPosition(sf::Vector2f(x, y));
    this->m_shape.setFillColor(buttonColors.idleColor);
    this->m_shape.setOutlineThickness(1.f);
    this->m_shape.setOutlineColor(outlineColors.idleColor);

    // Setup font
    this->m_text.setFont(font);
    this->m_text.setString(text);
    this->m_text.setCharacterSize(characterSize);
    this->m_text.setFillColor(m_textColors.idleColor);

    sf::FloatRect textRect = m_text.getLocalBounds();

    m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    m_text.setPosition(sf::Vector2f(x + width / 2, y + height / 2));
}

bool Button::isPressed() const {
    return this->m_state == ACTIVE;
}
void Button::setText(const std::string& text) {
    this->m_text.setString(text);
}

void Button::select(bool val) {
    this->m_state = val ? HOVER : IDLE;
}

void Button::handleEvent(sf::Event event) {
    if (event.type == sf::Event::MouseMoved) {
        if (this->m_shape.getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
            this->m_state = HOVER;
        }
        else {
            this->m_state = IDLE;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && this->m_state == HOVER) {
        this->m_state = ACTIVE;
    }
    else if (event.type == sf::Event::KeyReleased && 
             event.key.code == sf::Keyboard::Enter && this->m_state == HOVER) {
        this->m_state = ACTIVE;
    }

    if (this->m_state == IDLE)
    {
        this->m_shape.setFillColor(m_buttonColors.idleColor);
        this->m_shape.setOutlineColor(m_outlineColors.idleColor);
        this->m_text.setFillColor(m_textColors.idleColor);
    }
    else if (this->m_state == HOVER) {
        this->m_shape.setFillColor(m_buttonColors.hoverColor);
        this->m_shape.setOutlineColor(m_outlineColors.hoverColor);
        this->m_text.setFillColor(m_textColors.hoverColor);
    }
    else if (this->m_state == ACTIVE) {
        this->m_shape.setFillColor(m_buttonColors.activeColor);
        this->m_shape.setOutlineColor(m_outlineColors.activeColor);
        this->m_text.setFillColor(m_textColors.activeColor);

        // Call callback
        this->m_onPressedCallback();
    }

}
void Button::update(float elapsed) {
    if (this->m_state == HOVER) {
        float minScale = 0.9f;
        float maxScale = 1.1f;
        float periodSec = 1.4f;
        float percent = m_timeElapsed / periodSec;

        // Pulse text (grow)
        if (percent <= 0.5f) {
            float scale = (1.f - percent * 2.f) * minScale + percent * 2.f * maxScale;

            m_text.setScale(scale, scale);
        }

        // Pulse text (shrink)
        else {
            float scale = (1.f - (percent - 0.5f) * 2.f) * maxScale + (percent - 0.5f) * 2.f * minScale;

            m_text.setScale(scale, scale);
        }

        // Only keep track of a period's worth of time...
        m_timeElapsed = std::fmod(m_timeElapsed + elapsed, periodSec);
    }
    else {
        m_text.setScale(1.f, 1.f);
        
        this->m_timeElapsed = 0.f;
    }
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(this->m_shape);
    window.draw(this->m_text);
}