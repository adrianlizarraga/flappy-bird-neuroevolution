#include "AssetManager.h"
#include "Background.h"
#include "Bird.h"
#include "Ground.h"
#include "MLPNetwork.h"
#include "Pipe.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <memory>

using namespace std;

template <typename FloatNumericType> FloatNumericType random(FloatNumericType min, FloatNumericType max) {
    FloatNumericType randomFraction = ((FloatNumericType)rand()) / ((FloatNumericType)RAND_MAX);
    FloatNumericType range = max - min;

    return min + (range * randomFraction);
}

template <> int random(int min, int max) { return rand() % (max - min + 1) + min; }

void reset(Bird &bird, std::list<PipePair> &pipes) {
    bird.reset(200, 150);
    pipes.clear();
}

void cleanupPipes(std::list<PipePair> &pipes) {
    if (pipes.size() > 0) {
        auto firstPipeIterator = pipes.begin();
        sf::FloatRect firstPipeBbox = firstPipeIterator->getCombinedBoundingBox();

        if (firstPipeBbox.left + firstPipeBbox.width < 0.0f) {
            pipes.erase(firstPipeIterator);
        }
    }
}

int main() {
    srand(time(nullptr));

    int width = 1200;
    int height = 600;
    AssetManager assetManager;

    sf::Font& font = assetManager.getFont("data/trench.ttf");
    sf::Text scoreLabel;
    scoreLabel.setFont(font);
    scoreLabel.setString("Score: 0");
    scoreLabel.setCharacterSize(28);
    scoreLabel.setFillColor(sf::Color(219, 111, 57));
    scoreLabel.setOutlineThickness(1.0f);
    scoreLabel.setStyle(sf::Text::Bold);
    scoreLabel.setPosition(sf::Vector2f(width - 128, 0));

    // Create renderables
    float groundHeight = 128.0f;
    float backgroundHeight = 472.0f;
    Ground ground(sf::FloatRect(0.0f, height - groundHeight, width, groundHeight), assetManager);
    Background background(sf::FloatRect(0.0f, 0.0f, width, backgroundHeight), assetManager);
    Bird bird(200, 150, assetManager, &ground, &background);
    std::list<PipePair> pipes;

    sf::RenderWindow window(sf::VideoMode(width, height), "Flappy bird: live, die, and repeat");
    sf::Clock clock;
    int frame = 0;
    int pipeNumber = 0;
    int score = 0;

    // Draw loop @ 120fps
    window.setFramerateLimit(120);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    bird.flap();
                } else if (event.key.code == sf::Keyboard::R) {
                    reset(bird, pipes);
                    frame = 0;
                    score = 0;
                    pipeNumber = 0;
                    scoreLabel.setString("Score: 0");
                } else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

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

            if (!intersects && 
                (bird.getPosition().x > pipe.getX() + pipe.getWidth())) {
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
        cleanupPipes(pipes);

        if (collided) {
            reset(bird, pipes);
            frame = 0;
            score = 0;
            pipeNumber = 0;
            scoreLabel.setString("Score: 0");
        } else {
            frame++;
        }
    }

    return 0;
}