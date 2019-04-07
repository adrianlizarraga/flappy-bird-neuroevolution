#pragma once
#include "Renderable.h"
#include <SFML/Graphics.hpp>

class Pipe : public Renderable {
  private:
    sf::Sprite headSprite;
    sf::Sprite bodySprite;
    float vx;
    bool upsidedown;
    bool debug = false;

  public:
    Pipe(sf::FloatRect rect, const sf::Texture &headTexture, const sf::Texture &bodyTexture, bool upsidedown = false,
         float velocityX = -100.0f);

    ~Pipe();

    sf::FloatRect boundingBox() const;

    void update(float deltaT);
    void draw(sf::RenderWindow &window) const;
};

class PipePair : public Renderable {
  private:
    Pipe topPipe, bottomPipe;
    float gapY, gapHeight;
    bool debug = false;

  public:
    PipePair(sf::FloatRect rect, float gapY, float gapHeight, const sf::Texture &headTexture, const sf::Texture &bodyTexture,
             float velocityX = -100.0f);
    ~PipePair();

    sf::FloatRect boundingBox() const;
    sf::FloatRect getGapRect() const;

    void update(float deltaT);
    void draw(sf::RenderWindow &window) const;
};