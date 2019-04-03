#pragma once
#include "Renderable.h"
#include <SFML/Graphics.hpp>

class Pipe: public Renderable {
private:
    sf::Sprite headSprite;
    sf::Sprite bodySprite;
    float x, y;
    float height;
    bool upsidedown;
    bool debug = true;

public:
    Pipe(float x, float y, float height,
         const sf::Texture& headTexture, const sf::Texture& bodyTexture,
         bool upsidedown = false);

    ~Pipe();

    sf::FloatRect boundingBox() const;
    
    void update(float deltaT);
    void draw(sf::RenderWindow& window) const;
};