#include "Background.h"

Background::Background(float x, float y, int width, int height, const sf::Texture& texture){
    this->sprite.setTexture(texture);
    this->sprite.setTextureRect(sf::IntRect(0, 0, width, height));
    this->sprite.setPosition(x, y);
}

Background::~Background() {

}

sf::FloatRect Background::boundingBox() const {
    return this->sprite.getGlobalBounds();
}

void Background::update(float deltaT) {

}

void Background::draw(sf::RenderWindow& window) const {
    window.draw(this->sprite);
}