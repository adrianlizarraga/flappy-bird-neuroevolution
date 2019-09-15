#include "Menu.h"
#include "Game.h"
#include "AssetManager.h"
#include <string>

Menu::Menu(Game *game, sf::FloatRect rect) : m_game(game) {
    sf::Texture& texture = AssetManager::instance().getTexture("data/menubackground.png", false, false);
    sf::Vector2u size = texture.getSize();
    
    this->sprite.setTexture(texture);
    this->sprite.setPosition(rect.left, rect.top);
    this->sprite.setScale(rect.width / size.x, rect.height / size.y);

    sf::Vector2f position = this->sprite.getPosition();
    
    // Setup mode text
    sf::Font &font = AssetManager::instance().getFont("data/trench.ttf");
    m_modeText.setFont(font);
    m_modeText.setCharacterSize(32);
    m_modeText.setFillColor(sf::Color(219, 111, 57));
    m_modeText.setOutlineThickness(1.0f);
    m_modeText.setStyle(sf::Text::Bold);
    m_modeText.setPosition(sf::Vector2f(position.x + 64, position.y + 64));

    this->initModeText();
}

void Menu::initModeText() {
    m_modeText.setString("Mode: " + std::to_string(m_game->getMode()));
}

void Menu::handleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::P) {
            m_game->setPaused(!m_game->getPaused());
        } else if (event.key.code == sf::Keyboard::Num0) {
            m_game->setMode(0);
            this->initModeText();
        } else if (event.key.code == sf::Keyboard::Num1) {
            m_game->setMode(1);
            this->initModeText();
        } else if (event.key.code == sf::Keyboard::Num2) {
            m_game->setMode(2);
            this->initModeText();
        } else if (event.key.code == sf::Keyboard::Escape) {
            m_game->setPaused(false);
        }
    }
}

void Menu::update(float elapsed) {}

void Menu::draw(sf::RenderWindow &window) const {
    window.draw(this->sprite);
    window.draw(m_modeText);
}