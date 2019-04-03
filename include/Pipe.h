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
         bool upsidedown = false): x(x), y(y), height(height), upsidedown(upsidedown) {

        int headSpriteWidth = headTexture.getSize().x;
        int headSpriteHeight = headTexture.getSize().y;
        int bodySpriteWidth = bodyTexture.getSize().x;
        int bodySpriteHeight = height - headSpriteHeight;
        
        this->headSprite.setTexture(headTexture);
        this->headSprite.setPosition(x, y);
        
        if (upsidedown) {
            this->headSprite.setTextureRect(sf::IntRect(0, headSpriteHeight, headSpriteWidth, -headSpriteHeight));
        }

        float ybody = y + (upsidedown ? -bodySpriteHeight : headSpriteHeight);
        this->bodySprite.setTexture(bodyTexture);
        this->bodySprite.setPosition(x + (headSpriteWidth - bodySpriteWidth)/ 2.f, ybody);

        if (upsidedown) {
            this->bodySprite.setTextureRect(sf::IntRect(0, bodySpriteHeight, bodySpriteWidth, -bodySpriteHeight));
        }
        else {
            this->bodySprite.setTextureRect(sf::IntRect(0, 0, bodySpriteWidth, bodySpriteHeight));
        }
    }

    ~Pipe() {}

    sf::FloatRect boundingBox() const {
        sf::FloatRect bbox1 = this->headSprite.getGlobalBounds();
        sf::FloatRect bbox2 = this->bodySprite.getGlobalBounds();

        float xlt1 = bbox1.left;
        float xlt2 = bbox2.left;
        float ylt1 = bbox1.top;
        float ylt2 = bbox2.top;
        float xrb1 = bbox1.left + bbox1.width;
        float xrb2 = bbox2.left + bbox2.width;
        float yrb1 = bbox1.top + bbox1.height;
        float yrb2 = bbox2.top + bbox2.height;

        float xlt = xlt1 < xlt2 ? xlt1 : xlt2;
        float ylt = ylt1 < ylt2 ? ylt1 : ylt2;
        float xrb = xrb1 > xrb2 ? xrb1 : xrb2;
        float yrb = yrb1 > yrb2 ? yrb1 : yrb2;

        return sf::FloatRect(xlt, ylt, xrb - xlt, yrb - ylt);
    }
    
    void update(float deltaT) {}
    void draw(sf::RenderWindow& window) const {
        window.draw(this->headSprite);
        window.draw(this->bodySprite);

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