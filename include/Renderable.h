#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Renderable {
public:
    virtual ~Renderable() {}
    virtual sf::FloatRect boundingBox() const = 0;
    
    virtual bool intersects(const Renderable& other) const {
        return this->boundingBox().intersects(other.boundingBox());
    }
    
    virtual void update(float deltaT) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};