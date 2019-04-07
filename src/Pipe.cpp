#include "Pipe.h"

Pipe::Pipe(float x, float y, float height, const sf::Texture &headTexture, const sf::Texture &bodyTexture, 
           bool upsidedown, float velocityX) : x(x), y(y), height(height), upsidedown(upsidedown), vx(velocityX) {

    int headSpriteWidth = headTexture.getSize().x;
    int headSpriteHeight = headTexture.getSize().y;
    int bodySpriteWidth = bodyTexture.getSize().x;
    int bodySpriteHeight = height - headSpriteHeight;

    float yhead = upsidedown ? y + height : y;
    this->headSprite.setTexture(headTexture);
    this->headSprite.setPosition(x, yhead);

    if (upsidedown) {
        this->headSprite.setTextureRect(sf::IntRect(0, headSpriteHeight, headSpriteWidth, -headSpriteHeight));
        this->headSprite.setOrigin(0.0f, headSpriteHeight);
    }

    //float ybody = y + (upsidedown ? -bodySpriteHeight : headSpriteHeight);
    float ybody = yhead + (upsidedown ? -headSpriteHeight - bodySpriteHeight: headSpriteHeight);
    this->bodySprite.setTexture(bodyTexture);
    this->bodySprite.setPosition(x + (headSpriteWidth - bodySpriteWidth) / 2.f, ybody);

    if (upsidedown) {
        this->bodySprite.setTextureRect(sf::IntRect(0, bodySpriteHeight, bodySpriteWidth, -bodySpriteHeight));
    } else {
        this->bodySprite.setTextureRect(sf::IntRect(0, 0, bodySpriteWidth, bodySpriteHeight));
    }
}

Pipe::~Pipe() {}

sf::FloatRect Pipe::boundingBox() const {
    sf::FloatRect bbox1 = this->headSprite.getGlobalBounds();
    sf::FloatRect bbox2 = this->bodySprite.getGlobalBounds();

    return combineBoundingBoxes(bbox1, bbox2);
}

void Pipe::update(float deltaT) {
    float deltaX = this->vx * deltaT;

    this->x += deltaX;
    this->headSprite.move(deltaX, 0.0f);
    this->bodySprite.move(deltaX, 0.0f);
}

void Pipe::draw(sf::RenderWindow &window) const {
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