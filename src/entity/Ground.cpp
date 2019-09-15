#include "entity/Ground.h"
#include "asset/AssetManager.h"

Ground::Ground(sf::FloatRect rect, float elasticity): elasticity(elasticity) {
    sf::Texture& texture = AssetManager::instance().getTexture("data/ground64x128.png", false, true);
    
    this->sprite.setTexture(texture);
    this->sprite.setTextureRect(sf::IntRect(0, 0, rect.width, texture.getSize().y));
    this->sprite.setPosition(rect.left, rect.top);
    this->sprite.setScale(1.0f, rect.height / texture.getSize().y);
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