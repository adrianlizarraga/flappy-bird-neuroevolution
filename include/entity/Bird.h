#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
#include "entity/Renderable.h"
#include "MLPNetwork.h"
#include "entity/Pipe.h"

class Ground;
class Background;
class Pipe;

class Bird: public Renderable {
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float mass;
    float lift;
    const float g = 600.0f;
    const float b = 0.96f;
    bool _flap;
    const Ground* _ground;
    const Background* _background;

    int score;

    alai::MLPNetwork brain;

public:
    Bird(float x, float y,
         const Ground* ground,
         const Background* background,
         float mass = 0.05f,
         float lift = 1250.0f);

    Bird(float x, float y,
         const Ground* ground,
         const Background* background,
         const alai::MLPNetwork& brain,
         float mass = 0.05f, 
         float lift = 1250.0f);
    ~Bird();

    sf::Vector2f getPosition() const;

    sf::FloatRect getCombinedBoundingBox() const;
    std::vector<sf::FloatRect> getBoundingBoxes() const;

    int getScore() const;
    sf::Vector2f getVelocity() const;
    alai::MLPNetwork getBrain() const;

    void sense(const std::list<PipePair>& pipes, float width, float height);
    void flap();
    void update(float deltaT);
    void draw(sf::RenderWindow& window) const;
    void reset(float x, float y);

    bool checkPipeCollision(const PipePair& pipe);

    bool debug = false;
};