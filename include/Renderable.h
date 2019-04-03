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
    
    virtual bool contains(const Renderable& other) const {
        sf::FloatRect bbox1 = this->boundingBox();
        sf::FloatRect bbox2 = other.boundingBox();

        bool containsTL = bbox1.contains(sf::Vector2f(bbox2.left + 1, bbox2.top + 1));
        bool containsTR = bbox1.contains(sf::Vector2f(bbox2.left + bbox2.width - 1, bbox2.top + 1));
        bool containsBL = bbox1.contains(sf::Vector2f(bbox2.left + 1, bbox2.top + bbox2.height - 1));
        bool containsBR = bbox1.contains(sf::Vector2f(bbox2.left + bbox2.width - 1, bbox2.top + bbox2.height - 1));

        return containsTL && containsTR && containsBL && containsBR;
    }
    
    virtual void update(float deltaT) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};