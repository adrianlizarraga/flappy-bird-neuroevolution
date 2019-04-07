#pragma once
#include <SFML/Graphics.hpp>
#include "Renderable.h"

class Background: public Renderable {
private:
    sf::Sprite sprite;

public:
    Background(float x, float y, int width, int height, const sf::Texture& texture);
    ~Background();
    
    sf::FloatRect getCombinedBoundingBox() const;
    std::vector<sf::FloatRect> getBoundingBoxes() const;
    
    void update(float deltaT);
    void draw(sf::RenderWindow& window) const;
};