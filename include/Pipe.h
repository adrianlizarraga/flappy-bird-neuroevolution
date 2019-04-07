#pragma once
#include "AssetManager.h"
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
    Pipe(sf::FloatRect rect, AssetManager& assetManager, bool upsidedown = false, float velocityX = -100.0f);

    ~Pipe();

    sf::FloatRect getCombinedBoundingBox() const;
    std::vector<sf::FloatRect> getBoundingBoxes() const;

    void update(float deltaT);
    void draw(sf::RenderWindow &window) const;
};

class PipePair : public Renderable {
  private:
    Pipe topPipe, bottomPipe;
    float gapY, gapHeight;

  public:
    PipePair(sf::FloatRect rect, float gapY, float gapHeight, AssetManager& assetManager, float velocityX = -100.0f);
    ~PipePair();

    sf::FloatRect getGapRect() const;
    sf::FloatRect getCombinedBoundingBox() const;
    std::vector<sf::FloatRect> getBoundingBoxes() const;

    void update(float deltaT);
    void draw(sf::RenderWindow &window) const;

    bool debug = false;
};