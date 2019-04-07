#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

sf::FloatRect combineBoundingBoxes(const sf::FloatRect& bbox1, const sf::FloatRect& bbox2);

class Renderable {
public:
    virtual ~Renderable();
    virtual sf::FloatRect getCombinedBoundingBox() const = 0;
    virtual std::vector<sf::FloatRect> getBoundingBoxes() const = 0;
    
    virtual bool intersects(const Renderable& other) const;
    
    virtual void update(float deltaT) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};