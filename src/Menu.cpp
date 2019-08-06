#include "Menu.h"
#include "Game.h"

Menu::Menu(Game* game): m_game(game) {
}

void Menu::handleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::P) {
            m_game->setPaused(!m_game->getPaused());
        }
    }
}

void Menu::update(float elapsed) {

}

void Menu::draw(sf::RenderWindow& window) const {

}