#include "Game.h"
#include <iostream>

Game::Game(int width, int height, int fps, int mode)
    : m_width(width), m_height(height), m_mode(mode), m_window(sf::VideoMode(width, height), "Flappy bird: live, die, and repeat"),
      m_ground(sf::FloatRect(0.0f, height - m_groundHeight, width, m_groundHeight), m_assetManager),
      m_background(sf::FloatRect(0.0f, 0.0f, width, m_backgroundHeight), m_assetManager),
      m_bird(200, 150, m_assetManager, &m_ground, &m_background),
      m_randGapY(128, int(m_height - m_groundHeight - 200)),
      m_randGapHeight(64, 128) {

    // Setup score text
    sf::Font &font = m_assetManager.getFont("data/trench.ttf");
    m_scoreLabel.setFont(font);
    m_scoreLabel.setString("Score: 0");
    m_scoreLabel.setCharacterSize(28);
    m_scoreLabel.setFillColor(sf::Color(219, 111, 57));
    m_scoreLabel.setOutlineThickness(1.0f);
    m_scoreLabel.setStyle(sf::Text::Bold);
    m_scoreLabel.setPosition(sf::Vector2f(width - 128, 0));

    m_window.setFramerateLimit(fps);
}

void Game::addPipe() {
    const float pipeHeight = m_height - m_groundHeight;
    
    // Add a new pipe every few frames
    if (m_frame % 400 == 0) {
        float pipeWidth = 96.f;
        float gapY = m_randGapY(m_engine);
        float gapHeight = m_randGapHeight(m_engine);

        m_pipes.push_back(PipePair(m_pipeNumber, sf::FloatRect(m_width, 0.0f, pipeWidth, pipeHeight), gapY, gapHeight, m_assetManager));

        m_pipeNumber++;
    }
}

void Game::cleanupPipes() {
    if (m_pipes.size() > 0) {
        auto firstPipeIterator = m_pipes.begin();
        sf::FloatRect firstPipeBbox = firstPipeIterator->getCombinedBoundingBox();

        if (firstPipeBbox.left + firstPipeBbox.width < 0.0f) {
            m_pipes.erase(firstPipeIterator);
        }
    }
}

void Game::reset() {
    m_bird.reset(200, 150);
    m_pipes.clear();

    m_frame = 0;
    m_score = 0;
    m_pipeNumber = 0;
    m_scoreLabel.setString("Score: 0");
}

void Game::pollEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            m_window.close();
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space && m_mode == 0) {
                m_bird.flap();
            } else if (event.key.code == sf::Keyboard::R) {
                reset();
            } else if (event.key.code == sf::Keyboard::Escape) {
                m_window.close();
            }
        }
    }
}

void Game::draw() {
    m_window.clear(sf::Color::White);
    m_background.draw(m_window);
    m_bird.draw(m_window);
    m_ground.draw(m_window);

    for (auto &pipe : m_pipes) {
        pipe.draw(m_window);
    }

    m_window.draw(m_scoreLabel);

    m_window.display();
}

void Game::loop() {
    if (m_mode == 0) {
        this->loopPlayer();
    } else if (m_mode == 1) {
        this->loopTraining();
    } else if (m_mode == 2) {
        this->loopAI();
    }
}

void Game::loopPlayer() {
    const float pipeHeight = m_height - m_groundHeight;
    std::default_random_engine engine;
    std::uniform_int_distribution<int> randGapY(128, int(pipeHeight - 200));
    std::uniform_int_distribution<int> randGapHeight(64, 128);

    while (m_window.isOpen()) {
        
        // Handle user input
        this->pollEvents();

        // Add a pipe if necessary
        this->addPipe();

        float elapsed = m_clock.restart().asSeconds();

        // Update bird physics
        m_bird.update(elapsed);

        // Update pipe physics, check bird/pipe collision, and update score.
        bool collided = false;
        for (auto &pipe : m_pipes) {
            pipe.update(elapsed);

            bool intersects = m_bird.checkPipeCollision(pipe);
            collided = collided || intersects;

            m_score = m_bird.getScore();
            m_scoreLabel.setString("Score: " + std::to_string(m_score));
        }

        // Draw
        this->draw();

        // Clean up
        this->cleanupPipes();

        // Reset game if collided
        if (collided) {
            this->reset();
        } else {
            m_frame++;
        }
    }
}

void Game::loopTraining() {}

void Game::loopAI() {
    while (m_window.isOpen()) {
        
        // Handle user input...user not allowed to control the bird though...
        this->pollEvents();

        // Add a pipe if necessary.
        this->addPipe();

        float elapsed = m_clock.restart().asSeconds();

        // Update bird physics
        m_bird.update(elapsed);

        // Update pipe physics, check bird/pipe collision, and update score.
        bool collided = false;
        for (auto &pipe : m_pipes) {
            pipe.update(elapsed);

            bool intersects = m_bird.checkPipeCollision(pipe);
            collided = collided || intersects;

            m_score = m_bird.getScore();
            m_scoreLabel.setString("Score: " + std::to_string(m_score));
        }

        // Draw
        this->draw();

        // Clean up
        this->cleanupPipes();

        // Reset game if collided
        if (collided) {
            this->reset();
        } else {
            m_frame++;
        }

        // Let bird do its own thing with its neural network brain.
        m_bird.sense(m_pipes, m_width, m_height);
    }
}