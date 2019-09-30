#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

struct StatefulColorsDef {
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;
};

class Button {
private:
    enum ButtonState {
        IDLE = 0,
        HOVER = 1,
        ACTIVE = 2
    };

    ButtonState m_state;
    bool m_mouseMoved;
    int m_mouseX;
    int m_mouseY;

    float m_timeElapsed = 0.f;

    sf::RectangleShape m_shape;
    sf::Text m_text;

    StatefulColorsDef m_textColors;
    StatefulColorsDef m_buttonColors;
    StatefulColorsDef m_outlineColors;

    std::function<void()> m_onPressedCallback;

public:
    Button();
    Button(float x, float y, float width, float height, 
           const std::string& text, sf::Font& font, unsigned int characterSize,
           const StatefulColorsDef& textColors,
           const StatefulColorsDef& buttonColors,
           const StatefulColorsDef& outlineColors);

    Button(float x, float y, float width, float height, 
           const std::string& text, sf::Font& font, unsigned int characterSize,
           std::function<void()> onPressedCallback,
           const StatefulColorsDef& textColors,
           const StatefulColorsDef& buttonColors,
           const StatefulColorsDef& outlineColors);

    void initialize(float x, float y, float width, float height, 
                    const std::string& text, sf::Font& font, unsigned int characterSize,
                    std::function<void()> onPressedCallback,
                    const StatefulColorsDef& textColors,
                    const StatefulColorsDef& buttonColors,
                    const StatefulColorsDef& outlineColors);

    bool isPressed() const;
    void setText(const std::string& text);
    void select(bool val);

    void handleEvent(sf::Event event);
    void update(float elapsed);
    void draw(sf::RenderWindow& window) const;
};