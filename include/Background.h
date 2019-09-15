#pragma once
#include "Renderable.h"
#include "AssetManager.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Background: public Renderable {
private:
    sf::Sprite sprite;

public:
    Background(sf::FloatRect rect);
    ~Background();
    
    sf::FloatRect getCombinedBoundingBox() const;
    std::vector<sf::FloatRect> getBoundingBoxes() const;
    
    void update(float deltaT);
    void draw(sf::RenderWindow& window) const;
};