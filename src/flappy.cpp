#include "MLPNetwork.h"
#include "Bird.h"
#include "Ground.h"
#include "Background.h"
#include "Pipe.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;

int main() {
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
    Pipe pipe(350, height - groundTexture.getSize().y - 100.f, 100.f, pipeHeadTexture, pipeBodyTexture, false);


    sf::RenderWindow window(sf::VideoMode(width, height), "Flappy bird: live, die, and repeat");
    sf::Clock clock;
    
    window.setFramerateLimit(120);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    bird.flap();
                }
            }
        }

        window.clear(sf::Color::White);
        
        background.draw(window);

        float elapsed = clock.restart().asSeconds();
        bird.update(elapsed * 5.0);

        bird.draw(window);
        ground.draw(window);
        pipe.draw(window);

        window.display();
    }

    return 0;
}