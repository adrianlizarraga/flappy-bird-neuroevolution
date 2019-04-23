#include "Game.h"
#include <ctime>
#include <iostream>
#include <random>

Game::Game(int width, int height, int fps)
    : m_width(width), m_height(height), m_window(sf::VideoMode(width, height), "Flappy bird: live, die, and repeat"),
      m_ground(sf::FloatRect(0.0f, height - m_groundHeight, width, m_groundHeight), m_assetManager),
      m_background(sf::FloatRect(0.0f, 0.0f, width, m_backgroundHeight), m_assetManager), m_bird(200, 150, m_assetManager, &m_ground, &m_background) {

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

    srand(time(nullptr));
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
            if (event.key.code == sf::Keyboard::Space) {
                m_bird.flap();
            } else if (event.key.code == sf::Keyboard::R) {
                reset();
            } else if (event.key.code == sf::Keyboard::Escape) {
                m_window.close();
            }
        }
    }
}
void Game::loop(int mode) {
    if (mode == 0) {
        this->loopPlayer();
    }
    else if (mode == 1) {
        this->loopTraining();
    }
    else if (mode == 2) {
        this->loopAI();
    }
}

void Game::loopPlayer() {
    const float pipeHeight = m_height - m_groundHeight;
    std::default_random_engine engine;
    std::uniform_int_distribution<int> randGapY(128, int(pipeHeight - 200));
    std::uniform_int_distribution<int> randGapHeight(64, 128);

    while (m_window.isOpen()) {
        this->pollEvents();

        // Add a new pipe every few frames
        if (m_frame % 400 == 0) {
            float pipeWidth = 96.f;
            float gapY = randGapY(engine);
            float gapHeight = randGapHeight(engine);

            m_pipes.push_back(PipePair(m_pipeNumber, sf::FloatRect(m_width, 0.0f, pipeWidth, pipeHeight), gapY, gapHeight, m_assetManager));

            m_pipeNumber++;
        }

        float elapsed = m_clock.restart().asSeconds();

        // Update renderables
        m_bird.update(elapsed);

        bool collided = false;
        for (auto &pipe : m_pipes) {
            pipe.update(elapsed);

            bool intersects = pipe.intersects(m_bird);
            collided = collided || intersects;

            if (!intersects && (m_bird.getPosition().x > pipe.getX() + pipe.getWidth())) {
                int newscore = std::max(m_score, pipe.getNumber() + 1);

                if (m_score != newscore) {
                    m_score = newscore;
                    m_scoreLabel.setString("Score: " + std::to_string(m_score));
                }
            }
        }

        // Draw
        m_window.clear(sf::Color::White);
        m_background.draw(m_window);
        m_bird.draw(m_window);
        m_ground.draw(m_window);

        for (auto &pipe : m_pipes) {
            pipe.draw(m_window);
        }

        m_window.draw(m_scoreLabel);

        m_window.display();

        // Clean up and handle collision
        this->cleanupPipes();

        if (collided) {
            this->reset();
        } else {
            m_frame++;
        }
    }
}

void Game::loopTraining() {

}

void Game::loopAI() {

}