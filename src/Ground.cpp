#include "Ground.h"

Ground::Ground(float x, float y, int width, int height, const sf::Texture& texture,
             float elasticity): elasticity(elasticity) {
    this->sprite.setTexture(texture);
    this->sprite.setTextureRect(sf::IntRect(0, 0, width, height));
    this->sprite.setPosition(x, y);
}

Ground::~Ground() {

}

sf::FloatRect Ground::getCombinedBoundingBox() const {
    return this->sprite.getGlobalBounds();
}

std::vector<sf::FloatRect> Ground::getBoundingBoxes() const {
    return {this->sprite.getGlobalBounds()};
}

float Ground::getElasticity() const {
    return this->elasticity;
}

void Ground::update(float deltaT) {

}

void Ground::draw(sf::RenderWindow& window) const {
    window.draw(this->sprite);
}