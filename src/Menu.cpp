#include "Menu.h"
#include "Game.h"

Menu::Menu(Game *game) : m_game(game) {}

void Menu::handleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::P) {
            m_game->setPaused(!m_game->getPaused());
        } else if (event.key.code == sf::Keyboard::Num0) {
            m_game->setMode(0);
        } else if (event.key.code == sf::Keyboard::Num1) {
            m_game->setMode(1);
        } else if (event.key.code == sf::Keyboard::Num2) {
            m_game->setMode(2);
        } else if (event.key.code == sf::Keyboard::Escape) {
            m_game->setPaused(false);
        }
    }
}

void Menu::update(float elapsed) {}

void Menu::draw(sf::RenderWindow &window) const {}