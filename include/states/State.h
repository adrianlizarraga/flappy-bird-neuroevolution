#pragma once
#include <SFML/Graphics.hpp>

class State {
public:
    virtual ~State() {}
    virtual void handleEvent(sf::Event event) = 0;
    virtual void update(float elapsed) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};