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

    sf::Texture texture;
    if (!texture.loadFromFile("data/bird34x24.png")) {
        cerr << "Failed to load bird texture" << endl;
        return -1;
    }

    sf::Texture groundTexture;
    if (!groundTexture.loadFromFile("data/ground64x128.png")) {
        cerr << "Failed to load ground texture" << endl;
        return -1;
    }
    groundTexture.setRepeated(true);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("data/background300x472.png")) {
        cerr << "Failed to load background texture" << endl;
        return -1;
    }
    backgroundTexture.setRepeated(true);

    sf::Texture pipeHeadTexture;
    if (!pipeHeadTexture.loadFromFile("data/pipetop64x32.png")) {
        cerr << "Failed to load pipe top texture" << endl;
        return -1;
    }

    sf::Texture pipeBodyTexture;
    if (!pipeBodyTexture.loadFromFile("data/pipebody60x32.png")) {
        cerr << "Failed to load pipe body texture" << endl;
        return -1;
    }
    pipeBodyTexture.setRepeated(true);

    Ground ground(0.0f, height - groundTexture.getSize().y, width, groundTexture.getSize().y, groundTexture);
    Background background(0.0f, 0.0f, width, backgroundTexture.getSize().y, backgroundTexture);
    Bird bird(200, 150, texture, &ground, &background);
    std::list<PipePair> pipes;

    sf::RenderWindow window(sf::VideoMode(width, height), "Flappy bird: live, die, and repeat");
    sf::Clock clock;
    int frame = 0;

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
                } else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        if (frame % 400 == 0) {
            float pipeWidth = 64.0f * random(2, 4) / 2.0f;
            float pipeHeight = height - groundTexture.getSize().y;
            float gapY = random(128, int(pipeHeight - 200));
            float gapHeight = random(64, 128);
            pipes.push_back(PipePair(sf::FloatRect(width, 0.0f, pipeWidth, pipeHeight), gapY, gapHeight, pipeHeadTexture, pipeBodyTexture));
        }

        float elapsed = clock.restart().asSeconds();
        bird.debug = false;

        bird.update(elapsed);

        bool collided = false;
        for (auto &pipe : pipes) {
            pipe.update(elapsed);

            collided = collided || pipe.intersects(bird);
        }

        window.clear(sf::Color::White);
        background.draw(window);
        bird.draw(window);
        ground.draw(window);

        for (auto &pipe : pipes) {
            pipe.draw(window);
        }

        window.display();

        cleanupPipes(pipes);

        if (collided) {
            reset(bird, pipes);
            frame = 0;
        } else {
            frame++;
        }
    }

    return 0;
}