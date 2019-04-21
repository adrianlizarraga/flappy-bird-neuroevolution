#include "Game.h"
#include "utils.h"
#include <ctime>
#include <iostream>

Game::Game(int width, int height, int fps)
    : width(width), height(height), window(sf::VideoMode(width, height), "Flappy bird: live, die, and repeat"),
      ground(sf::FloatRect(0.0f, height - groundHeight, width, groundHeight), assetManager),
      background(sf::FloatRect(0.0f, 0.0f, width, backgroundHeight), assetManager), bird(200, 150, assetManager, &ground, &background) {

    // Setup score text
    sf::Font &font = assetManager.getFont("data/trench.ttf");
    scoreLabel.setFont(font);
    scoreLabel.setString("Score: 0");
    scoreLabel.setCharacterSize(28);
    scoreLabel.setFillColor(sf::Color(219, 111, 57));
    scoreLabel.setOutlineThickness(1.0f);
    scoreLabel.setStyle(sf::Text::Bold);
    scoreLabel.setPosition(sf::Vector2f(width - 128, 0));

    window.setFramerateLimit(fps);

    srand(time(nullptr));
}

void Game::cleanupPipes() {
    if (pipes.size() > 0) {
        auto firstPipeIterator = pipes.begin();
        sf::FloatRect firstPipeBbox = firstPipeIterator->getCombinedBoundingBox();

        if (firstPipeBbox.left + firstPipeBbox.width < 0.0f) {
            pipes.erase(firstPipeIterator);
        }
    }
}

void Game::reset() {
    bird.reset(200, 150);
    pipes.clear();
}

void Game::pollEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                bird.flap();
            } else if (event.key.code == sf::Keyboard::R) {
                reset();
                frame = 0;
                score = 0;
                pipeNumber = 0;
                scoreLabel.setString("Score: 0");
            } else if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
    }
}

void Game::loop() {
    while (window.isOpen()) {
        this->pollEvents();

        // Add a new pipe every few frames
        if (frame % 400 == 0) {
            float pipeWidth = 64.0f * random(2, 4) / 2.0f;
            float pipeHeight = height - groundHeight;
            float gapY = random(128, int(pipeHeight - 200));
            float gapHeight = random(64, 128);

            pipes.push_back(PipePair(pipeNumber, sf::FloatRect(width, 0.0f, pipeWidth, pipeHeight), gapY, gapHeight, assetManager));

            pipeNumber++;
        }

        float elapsed = clock.restart().asSeconds();

        // Update renderables
        bird.update(elapsed);

        bool collided = false;
        for (auto &pipe : pipes) {
            pipe.update(elapsed);

            bool intersects = pipe.intersects(bird);
            collided = collided || intersects;

            if (!intersects && (bird.getPosition().x > pipe.getX() + pipe.getWidth())) {
                int newscore = std::max(score, pipe.getNumber() + 1);

                if (score != newscore) {
                    score = newscore;
                    scoreLabel.setString("Score: " + std::to_string(score));
                }
            }
        }

        // Draw
        window.clear(sf::Color::White);
        background.draw(window);
        bird.draw(window);
        ground.draw(window);

        for (auto &pipe : pipes) {
            pipe.draw(window);
        }

        window.draw(scoreLabel);

        window.display();

        // Clean up and handle collision
        this->cleanupPipes();

        if (collided) {
            this->reset();
            frame = 0;
            score = 0;
            pipeNumber = 0;
            scoreLabel.setString("Score: 0");
        } else {
            frame++;
        }
    }
}