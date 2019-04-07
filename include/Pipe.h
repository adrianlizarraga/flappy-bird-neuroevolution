#pragma once
#include "Renderable.h"
#include <SFML/Graphics.hpp>

class Pipe : public Renderable {
  private:
    sf::Sprite headSprite;
    sf::Sprite bodySprite;
    float x, y;
    float vx;
    float height;
    bool upsidedown;
    bool debug = false;

  public:
    Pipe(float x, float y, float height, const sf::Texture &headTexture, const sf::Texture &bodyTexture, bool upsidedown = false,
         float velocityX = -100.0f);

    ~Pipe();

    sf::FloatRect boundingBox() const;

    void update(float deltaT);
    void draw(sf::RenderWindow &window) const;
};

class PipePair : public Renderable {
  private:
    Pipe topPipe;
    Pipe bottomPipe;
    float x;
    float y;
    float vx;
    float height;
    float gapY;
    float gapHeight;
    bool debug = true;

  public:
    PipePair(float x, float y, float height, float gapY, float gapHeight, const sf::Texture &headTexture, const sf::Texture &bodyTexture,
             float velocityX = -100.0f)
        : x(x), y(y), vx(velocityX), height(height), gapY(gapY), gapHeight(gapHeight),
          topPipe(x, y, gapY - y, headTexture, bodyTexture, true, velocityX),
          bottomPipe(x, gapY + gapHeight, y + height - (gapY + gapHeight), headTexture, bodyTexture, false, velocityX) {
              std::cout << gapY << " " << gapY - y << std::endl;
              
              sf::FloatRect bbox = this->boundingBox();
              std::cout << bbox.left << " " << bbox.top << " " << bbox.width << " " << bbox.height << std::endl;
          }

    ~PipePair() {}

    sf::FloatRect boundingBox() const {
        sf::FloatRect topBbox = this->topPipe.boundingBox();
        sf::FloatRect bottomBbox = this->bottomPipe.boundingBox();

        return combineBoundingBoxes(topBbox, bottomBbox);
    }

    void update(float deltaT) {
        this->topPipe.update(deltaT);
        this->bottomPipe.update(deltaT);
    }

    void draw(sf::RenderWindow &window) const {
        this->topPipe.draw(window);
        this->bottomPipe.draw(window);

        if (this->debug) {
            sf::FloatRect bbox = this->boundingBox();
            sf::RectangleShape r(sf::Vector2f(bbox.width, bbox.height));
            r.setPosition(bbox.left, bbox.top);
            r.setOutlineThickness(1.0f);
            r.setOutlineColor(sf::Color::Red);
            r.setFillColor(sf::Color::Transparent);
            window.draw(r);
        }
    }
};