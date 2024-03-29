#pragma once
#include <SFML/Graphics.hpp>

class Game;

class Menu {
private:
    Game* m_game;
    sf::Sprite sprite;
    sf::Text m_modeText;
    sf::Text m_modeSubtext;

    void initModeText();
public:
    Menu(Game* game, sf::FloatRect rect);

    void handleEvent(sf::Event event);
    void update(float elapsed);
    void draw(sf::RenderWindow& window) const;
};