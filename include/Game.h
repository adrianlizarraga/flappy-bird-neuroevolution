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
    int m_width, m_height;
    float m_groundHeight = 128.0f;
    float m_backgroundHeight = 472.0f;

    int m_frame = 0;
    int m_pipeNumber = 0;
    int m_score = 0;

    sf::RenderWindow m_window;
    sf::Clock m_clock;
    
    AssetManager m_assetManager;

    sf::Text m_scoreLabel;
    Ground m_ground;
    Background m_background;
    Bird m_bird;
    std::list<PipePair> m_pipes;

    void cleanupPipes();
    void reset();
    void pollEvents();
    void loopPlayer();
    void loopTraining();
    void loopAI();

public:
    Game(int width, int height, int fps = 120);
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void loop(int mode = 0);
};