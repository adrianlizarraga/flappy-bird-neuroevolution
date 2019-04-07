#include "Pipe.h"

////////////////////////////////////////////
// Pipe
///////////////////////////////////////////
Pipe::Pipe(sf::FloatRect rect, const sf::Texture &headTexture, const sf::Texture &bodyTexture, bool upsidedown, float velocityX)
    : upsidedown(upsidedown), vx(velocityX) {

    sf::Vector2u headTextureSize = headTexture.getSize();
    sf::Vector2u bodyTextureSize = bodyTexture.getSize();

    float widthScale = rect.width / headTextureSize.x;
    sf::Vector2f headSpriteSize(rect.width, headTextureSize.y);
    sf::Vector2f bodySpriteSize(bodyTextureSize.x * widthScale, rect.height - headSpriteSize.y);

    float yhead = upsidedown ? rect.top + rect.height : rect.top;
    this->headSprite.setTexture(headTexture);
    this->headSprite.setPosition(rect.left, yhead);
    this->headSprite.setScale(widthScale, 1.0f);

    if (upsidedown) {
        this->headSprite.setTextureRect(sf::IntRect(0, headTextureSize.y, headTextureSize.x, -headTextureSize.y));
        this->headSprite.setOrigin(0.0f, headSpriteSize.y);
    }

    float ybody = yhead + (upsidedown ? -headSpriteSize.y - bodySpriteSize.y : headSpriteSize.y);
    this->bodySprite.setTexture(bodyTexture);
    this->bodySprite.setPosition(rect.left + (headSpriteSize.x - bodySpriteSize.x) / 2.f, ybody);
    this->bodySprite.setScale(widthScale, 1.0f);

    if (upsidedown) {
        this->bodySprite.setTextureRect(sf::IntRect(0, bodySpriteSize.y, bodyTextureSize.x, -bodySpriteSize.y));
    } else {
        this->bodySprite.setTextureRect(sf::IntRect(0, 0, bodyTextureSize.x, bodySpriteSize.y));
    }
}

// ------------------------------------------------------------
Pipe::~Pipe() {}

// ------------------------------------------------------------
sf::FloatRect Pipe::boundingBox() const {
    sf::FloatRect bbox1 = this->headSprite.getGlobalBounds();
    sf::FloatRect bbox2 = this->bodySprite.getGlobalBounds();

    return combineBoundingBoxes(bbox1, bbox2);
}

// ------------------------------------------------------------
void Pipe::update(float deltaT) {
    float deltaX = this->vx * deltaT;

    this->headSprite.move(deltaX, 0.0f);
    this->bodySprite.move(deltaX, 0.0f);
}

// ------------------------------------------------------------
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

////////////////////////////////////////////
// PipePair
///////////////////////////////////////////

PipePair::PipePair(sf::FloatRect rect, float gapY, float gapHeight, const sf::Texture &headTexture, const sf::Texture &bodyTexture,
                   float velocityX)
    : gapY(gapY), gapHeight(gapHeight),
      topPipe(sf::FloatRect(rect.left, rect.top, rect.width, gapY - rect.top), headTexture, bodyTexture, true, velocityX),
      bottomPipe(sf::FloatRect(rect.left, gapY + gapHeight, rect.width, rect.top + rect.height - (gapY + gapHeight)), headTexture,
                 bodyTexture, false, velocityX) {}

PipePair::~PipePair() {}

sf::FloatRect PipePair::boundingBox() const {
    sf::FloatRect topBbox = this->topPipe.boundingBox();
    sf::FloatRect bottomBbox = this->bottomPipe.boundingBox();

    return combineBoundingBoxes(topBbox, bottomBbox);
}

sf::FloatRect PipePair::getGapRect() const {
    sf::FloatRect bbox = this->boundingBox();

    return sf::FloatRect(bbox.left, this->gapY, bbox.width, this->gapHeight);
}

void PipePair::update(float deltaT) {
    this->topPipe.update(deltaT);
    this->bottomPipe.update(deltaT);
}

void PipePair::draw(sf::RenderWindow &window) const {
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