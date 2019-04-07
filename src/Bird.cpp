#include "Bird.h"
#include "Background.h"
#include "Ground.h"
#include <iostream>

Bird::Bird(float x, float y, 
           AssetManager& assetManager,
           const Ground* ground,
           const Background* background,
           float mass, float lift): 
    position(x, y), velocity(0.0f, 0.0f), acceleration(0.0f, 0.0f), 
    mass(mass), lift(lift), _ground(ground), _background(background) {
    
    sf::Texture& texture = assetManager.getTexture("data/bird34x24.png");
    sf::Vector2u size = texture.getSize();
    
    this->sprite.setPosition(x, y);
    this->sprite.setTexture(texture);
    this->sprite.setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));

    this->_flap = false;
}

Bird::~Bird() {

}

void Bird::reset(float x, float y) {
    this->position.x = x;
    this->position.y = y;
    this->velocity.x = 0.0f;
    this->velocity.y = 0.0f;
    
    this->sprite.setPosition(x, y);
}

sf::Vector2f Bird::getPosition() const {
    return this->position;
}

sf::FloatRect Bird::getCombinedBoundingBox() const {
    return this->sprite.getGlobalBounds();
}

std::vector<sf::FloatRect> Bird::getBoundingBoxes() const {
    return {this->sprite.getGlobalBounds()};
}

void Bird::flap() {

    // Only allow flapping if bird is almost about to start falling again.
    if (this->velocity.y >= -55.f) {
        this->_flap = true;
    }
}

void Bird::update(float deltaT) {
    this->sprite.setRotation(0);
    this->acceleration.x = 0.0f;
    this->acceleration.y = 0.0f;
    
    bool hitGround = this->intersects(*this->_ground);
    bool hitCeiling = this->getBoundingBoxes()[0].top < this->_background->getBoundingBoxes()[0].top;
    
    // Force velocity to zero if bird touches ceiling.
    if (hitCeiling) {
        this->velocity.y = 0.0f;
    }

    // Reflect velocity if bird hit the ground
    if (hitGround) {
        this->velocity.y = -abs(this->velocity.y * this->_ground->getElasticity());
    }
    
    // Otherwise, apply force of gravity and drag if bird is in the air.
    else {
        this->acceleration += sf::Vector2f(0.0f, this->g); // Gravity
        this->acceleration += -this->velocity * float(this->b); // Air drag
    }
    
    // Apply lift force if bird is flapping and not hitting the ceiling.
    if (this->_flap && !hitCeiling) {
        this->acceleration += sf::Vector2f(0.0f, -this->lift / this->mass);
    }

    // Update velocity and position
    this->velocity += this->acceleration * deltaT;
    this->position += this->velocity * deltaT;

    // Set the sprite's position for rendering
    this->sprite.setPosition(this->position);

    // Set the sprite's rotation based on its speed and general location.
    if (hitCeiling || hitGround) {
        this->sprite.setRotation(0.f);
    }
    else if (this->velocity.y > 100.0f) {
        this->sprite.setRotation(30.0f);
    }
    else if (this->velocity.y < -70.0f) {
        this->sprite.setRotation(-30.0f);
    }
    else {
        this->sprite.setRotation(0.f);
    }

    this->_flap = false;
}

void Bird::draw(sf::RenderWindow& window) const {
    window.draw(this->sprite);

    if (this->debug) {
        sf::FloatRect bbox = this->sprite.getGlobalBounds();
        sf::RectangleShape r(sf::Vector2f(bbox.width, bbox.height));
        r.setPosition(bbox.left, bbox.top);
        r.setOutlineThickness(1.0f);
        r.setOutlineColor(sf::Color::Red);
        r.setFillColor(sf::Color::Transparent);
        window.draw(r);
    }
}