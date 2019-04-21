#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Background.h"
#include "Bird.h"
#include "Ground.h"
#include "Pipe.h"

class Game {
private:
    int width, height;
    float groundHeight = 128.0f;
    float backgroundHeight = 472.0f;

    int frame = 0;
    int pipeNumber = 0;
    int score = 0;

    sf::RenderWindow window;
    sf::Clock clock;
    
    AssetManager assetManager;

    sf::Text scoreLabel;
    Ground ground;
    Background background;
    Bird bird;
    std::list<PipePair> pipes;

    void cleanupPipes();
    void reset();
    void pollEvents();

public:
    Game(int width, int height, int fps = 120);
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void loop();
};