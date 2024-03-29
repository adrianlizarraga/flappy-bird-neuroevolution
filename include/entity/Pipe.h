#pragma once
#include "entity/Renderable.h"
#include <SFML/Graphics.hpp>

class Pipe : public Renderable {
  private:
    sf::Sprite headSprite;
    sf::Sprite bodySprite;
    float x, vx, width;
    bool upsidedown;
    bool debug = false;

  public:
    Pipe(sf::FloatRect rect, bool upsidedown = false, float velocityX = -100.0f);

    ~Pipe();
    
    float getX() const;
    float getWidth() const;
    
    sf::FloatRect getCombinedBoundingBox() const;
    std::vector<sf::FloatRect> getBoundingBoxes() const;

    void update(float deltaT);
    void draw(sf::RenderWindow &window) const;
};

class PipePair : public Renderable {
  private:
    Pipe topPipe, bottomPipe;
    float gapY, gapHeight;
    int number;

  public:
    PipePair(int number, sf::FloatRect rect, float gapY, float gapHeight, float velocityX = -100.0f);
    ~PipePair();

    int getNumber() const;
    float getX() const;
    float getWidth() const;

    sf::FloatRect getGapRect() const;
    sf::FloatRect getCombinedBoundingBox() const;
    std::vector<sf::FloatRect> getBoundingBoxes() const;

    void update(float deltaT);
    void draw(sf::RenderWindow &window) const;

    bool debug = false;
};