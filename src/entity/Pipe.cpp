#include "entity/Pipe.h"
#include "asset/AssetManager.h"
#include <algorithm>

////////////////////////////////////////////
// Pipe
///////////////////////////////////////////
Pipe::Pipe(sf::FloatRect rect, bool upsidedown, float velocityX) : x(rect.left), vx(velocityX), width(rect.width), upsidedown(upsidedown) {

    sf::Texture &headTexture = AssetManager::instance().getTexture("data/pipetop64x32.png");
    sf::Texture &bodyTexture = AssetManager::instance().getTexture("data/pipebody60x32.png", false, true);

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
float Pipe::getX() const {
    return this->x;
}

// ------------------------------------------------------------
float Pipe::getWidth() const {
    return this->width;
}

// ------------------------------------------------------------
sf::FloatRect Pipe::getCombinedBoundingBox() const {
    return combineBoundingBoxes(this->getBoundingBoxes()[0], this->getBoundingBoxes()[1]);
}

// ------------------------------------------------------------
std::vector<sf::FloatRect> Pipe::getBoundingBoxes() const {
    sf::FloatRect bbox1 = this->headSprite.getGlobalBounds();
    sf::FloatRect bbox2 = this->bodySprite.getGlobalBounds();

    return {bbox1, bbox2};
}

// ------------------------------------------------------------
void Pipe::update(float deltaT) {
    float deltaX = this->vx * deltaT;

    // Cache x so we don't have to always retrieve bounding box.
    this->x += deltaX;

    this->headSprite.move(deltaX, 0.0f);
    this->bodySprite.move(deltaX, 0.0f);
}

// ------------------------------------------------------------
void Pipe::draw(sf::RenderWindow &window) const {
    window.draw(this->headSprite);
    window.draw(this->bodySprite);

    if (this->debug) {
        std::vector<sf::FloatRect> bboxes = this->getBoundingBoxes();

        for (auto &bbox : bboxes) {
            sf::RectangleShape r(sf::Vector2f(bbox.width, bbox.height));
            r.setPosition(bbox.left, bbox.top);
            r.setOutlineThickness(1.0f);
            r.setOutlineColor(sf::Color::Red);
            r.setFillColor(sf::Color::Transparent);
            window.draw(r);
        }
    }
}

////////////////////////////////////////////
// PipePair
///////////////////////////////////////////

PipePair::PipePair(int number, sf::FloatRect rect, float gapY, float gapHeight, float velocityX)
    : number(number), gapY(gapY), gapHeight(gapHeight),
      topPipe(sf::FloatRect(rect.left, rect.top, rect.width, gapY - rect.top), true, velocityX),
      bottomPipe(sf::FloatRect(rect.left, gapY + gapHeight, rect.width, rect.top + rect.height - (gapY + gapHeight)), false,
                 velocityX) {}

// ------------------------------------------------------------
PipePair::~PipePair() {}

// ------------------------------------------------------------
int PipePair::getNumber() const {
    return this->number;
}

// ------------------------------------------------------------
float PipePair::getX() const {
    return this->topPipe.getX();
}

// ------------------------------------------------------------
float PipePair::getWidth() const {
    return this->topPipe.getWidth();
}

// ------------------------------------------------------------
sf::FloatRect PipePair::getCombinedBoundingBox() const {
    sf::FloatRect topBbox = this->topPipe.getCombinedBoundingBox();
    sf::FloatRect bottomBbox = this->bottomPipe.getCombinedBoundingBox();

    return combineBoundingBoxes(topBbox, bottomBbox);
}

// ------------------------------------------------------------
sf::FloatRect PipePair::getGapRect() const {
    sf::FloatRect bbox = this->getCombinedBoundingBox();

    return sf::FloatRect(bbox.left, this->gapY, bbox.width, this->gapHeight);
}

// ------------------------------------------------------------
std::vector<sf::FloatRect> PipePair::getBoundingBoxes() const {
    std::vector<sf::FloatRect> topBBoxes = this->topPipe.getBoundingBoxes();
    std::vector<sf::FloatRect> botBBoxes = this->bottomPipe.getBoundingBoxes();
    std::vector<sf::FloatRect> bboxes;

    for (auto &bbox : topBBoxes) {
        bboxes.push_back(bbox);
    }

    for (auto &bbox : botBBoxes) {
        bboxes.push_back(bbox);
    }

    return bboxes;
}

// ------------------------------------------------------------
void PipePair::update(float deltaT) {
    this->topPipe.update(deltaT);
    this->bottomPipe.update(deltaT);
}

// ------------------------------------------------------------
void PipePair::draw(sf::RenderWindow &window) const {
    this->topPipe.draw(window);
    this->bottomPipe.draw(window);

    if (this->debug) {
        std::vector<sf::FloatRect> bboxes = this->getBoundingBoxes();

        for (auto &bbox : bboxes) {
            sf::RectangleShape r(sf::Vector2f(bbox.width, bbox.height));
            r.setPosition(bbox.left, bbox.top);
            r.setOutlineThickness(1.0f);
            r.setOutlineColor(sf::Color::Red);
            r.setFillColor(sf::Color::Transparent);
            window.draw(r);
        }
    }
}