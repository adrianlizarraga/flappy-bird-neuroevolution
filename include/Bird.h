#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Renderable.h"

class Ground;
class Background;

class Bird: public Renderable {
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float mass;
    float lift;
    const float g = 80.0f;
    const float b = 0.96;
    const bool debug = false;
    bool _flap;
    const Ground* _ground;
    const Background* _background;

    void reset();

public:
    Bird(float x, float y, 
         const sf::Texture& texture,
         const Ground* ground,
         const Background* background,
         float mass = 1.0f, 
         float lift = 2500.0f);
    ~Bird();

    sf::FloatRect boundingBox() const;

    void flap();
    void update(float deltaT);
    void draw(sf::RenderWindow& window) const;
};