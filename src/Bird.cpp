#include "Bird.h"
#include "Background.h"
#include "Ground.h"
#include <iostream>

Bird::Bird(float x, float y, sf::Texture& texture, const Ground *ground, const Background *background, float mass, float lift)
    : Bird(x, y, texture, ground, background, alai::MLPNetwork(5, {5,1}), mass, lift) {
}

Bird::Bird(float x, float y, sf::Texture& texture, const Ground *ground, const Background *background,
           const alai::MLPNetwork &brain, float mass, float lift)
    : position(x, y), velocity(0.0f, 0.0f), acceleration(0.0f, 0.0f), mass(mass), lift(lift), _ground(ground), _background(background),
      brain(brain), score(0) {
    sf::Vector2u size = texture.getSize();

    this->sprite.setPosition(x, y);
    this->sprite.setTexture(texture);
    this->sprite.setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));

    this->_flap = false;
}

Bird::~Bird() {}

void Bird::reset(float x, float y) {
    this->position.x = x;
    this->position.y = y;
    this->velocity.x = 0.0f;
    this->velocity.y = 0.0f;

    this->sprite.setPosition(x, y);

    this->score = 0;

    this->brain.randomizeWeights();
}

bool Bird::checkPipeCollision(const PipePair& pipe) {
    bool collided = pipe.intersects(*this);

    if (!collided && (this->position.x > pipe.getX() + pipe.getWidth())) {
        this->score = std::max(this->score, pipe.getNumber() + 1);
    }

    return collided;
}

int Bird::getScore() const {
    return this->score;
}

sf::Vector2f Bird::getVelocity() const {
    return this->velocity;
}

alai::MLPNetwork Bird::getBrain() const {
    return this->brain;
}

sf::Vector2f Bird::getPosition() const { return this->position; }

sf::FloatRect Bird::getCombinedBoundingBox() const { return this->sprite.getGlobalBounds(); }

std::vector<sf::FloatRect> Bird::getBoundingBoxes() const { return {this->sprite.getGlobalBounds()}; }

void Bird::sense(const std::list<PipePair> &pipes, float width, float height) {
    
    // Find closest pipe infront of bird.
    PipePair pipe = pipes.back();
    
    for (std::list<PipePair>::const_iterator it = pipes.begin(); it != pipes.end(); it++) {
        sf::FloatRect gap = it->getGapRect();
        sf::FloatRect closestGap = pipe.getGapRect();
        float gapRightX = gap.left + gap.width;
        float closestGapRightX = closestGap.left + closestGap.width;

        if (gapRightX < closestGapRightX && this->position.x <= gapRightX) {
            pipe = *it;
        }
    }

    sf::FloatRect gapRect = pipe.getGapRect();

    // Use the gap coordinates and the bird's upward velocity as inputs to the neural network.    
    std::vector<double> inputs(5);

    inputs[0] = (gapRect.left - this->position.x) / width;
    inputs[1] = (gapRect.left + gapRect.width - this->position.x) / width;
    inputs[2] = -(gapRect.top - this->position.y) / height;
    inputs[3] = -(gapRect.top + gapRect.height - this->position.y) / height;
    inputs[4] = -this->velocity.y / 500.f;

    // Use our big bird brain to determine if should flap.
    std::vector<double> flapProb = this->brain.compute(inputs);

    if (flapProb[0] >= 0.5) {
        this->flap();
    }
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
        this->acceleration += sf::Vector2f(0.0f, this->g);      // Gravity
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
    } else if (this->velocity.y > 100.0f) {
        this->sprite.setRotation(30.0f);
    } else if (this->velocity.y < -70.0f) {
        this->sprite.setRotation(-30.0f);
    } else {
        this->sprite.setRotation(0.f);
    }

    this->_flap = false;
}

void Bird::draw(sf::RenderWindow &window) const {
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