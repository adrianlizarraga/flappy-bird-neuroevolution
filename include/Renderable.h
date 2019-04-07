#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

sf::FloatRect combineBoundingBoxes(const sf::FloatRect& bbox1, const sf::FloatRect& bbox2);

class Renderable {
public:
    virtual ~Renderable();
    virtual sf::FloatRect boundingBox() const = 0;
    
    virtual bool intersects(const Renderable& other) const;
    
    virtual void update(float deltaT) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};