#include "Background.h"

Background::Background(sf::FloatRect rect){
    sf::Texture& texture = AssetManager::instance().getTexture("data/background300x472.png", false, true);
    
    this->sprite.setTexture(texture);
    this->sprite.setTextureRect(sf::IntRect(0, 0, rect.width, texture.getSize().y));
    this->sprite.setPosition(rect.left, rect.top);
    this->sprite.setScale(1.0f, rect.height / texture.getSize().y);
}

Background::~Background() {

}

sf::FloatRect Background::getCombinedBoundingBox() const {
    return this->sprite.getGlobalBounds();
}

std::vector<sf::FloatRect> Background::getBoundingBoxes() const {
    return {this->sprite.getGlobalBounds()};
}

void Background::update(float deltaT) {

}

void Background::draw(sf::RenderWindow& window) const {
    window.draw(this->sprite);
}