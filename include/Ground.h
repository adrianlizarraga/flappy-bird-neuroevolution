#pragma once
#include <SFML/Graphics.hpp>
#include "Renderable.h"
#include "AssetManager.h"

class Ground: public Renderable {
private:
    sf::Sprite sprite;
    float elasticity;

public:
    Ground(sf::FloatRect rect, AssetManager& assetManager, float elasticity = 0.7f);
    ~Ground();
    
    sf::FloatRect getCombinedBoundingBox() const;
    std::vector<sf::FloatRect> getBoundingBoxes() const;

    float getElasticity() const;
    
    void update(float deltaT);
    void draw(sf::RenderWindow& window) const;
};