#pragma once
#include <SFML/Graphics.hpp>

class Game;

class Menu {
private:
    Game* m_game; 
public:
    Menu(Game* game);

    void handleEvent(sf::Event event);
    void update(float elapsed);
    void draw(sf::RenderWindow& window) const;
};