#include "Game.h"
#include "states/TitleScreenState.h"
#include "states/PlayState.h"
#include "states/CPUPlayState.h"
#include "states/TrainState.h"
#include <chrono>
#include <algorithm>
#include <iostream>

Game::Game(int width, int height, int fps, int mode)
    : m_width(width), m_height(height), m_window(sf::VideoMode(width, height), "Flappy bird: live, die, and repeat"), m_mode(mode),
      m_menu(this, sf::FloatRect(width / 2.f - width / 4.f, height / 2.f - height / 4.f, width / 2.f, height / 2.f)) {

    m_window.setFramerateLimit(fps);

    /*if (mode == 0) {
        m_stateStack.push(std::make_shared<PlayState>(width, height, m_stateStack));
    }
    else if (mode == 1) {
        m_stateStack.push(std::make_shared<TrainState>(width, height, m_stateStack));
    }
    else if (mode == 2) {
        m_stateStack.push(std::make_shared<CPUPlayState>(width, height, m_stateStack));
    }*/

    m_stateStack.push(std::make_shared<TitleScreenState>(width, height, m_stateStack));
}
void Game::reset() {

}

bool Game::getPaused() const { return m_paused; }

void Game::setPaused(bool paused) { m_paused = paused; }

void Game::setMode(int mode) {
    m_mode = mode;
    this->reset();
}

int Game::getMode() const { return m_mode; }

void Game::pollEvents() {
    sf::Event event;

    while (m_window.pollEvent(event)) {

        // Exit app.
        if (event.type == sf::Event::Closed) {
            m_window.close();
        } 
        
        // Delegate event to state stack.
        else {
            m_stateStack.handleEvent(event);
        }
    }
}

void Game::update() {
    float elapsed = m_clock.restart().asSeconds();

    m_stateStack.update(elapsed);
}

void Game::draw() {
    m_window.clear(sf::Color::White);
    m_stateStack.draw(m_window);
    m_window.display();
}

void Game::loop() {
    while (m_window.isOpen()) {
        this->pollEvents();
        this->update();
        this->draw();
    }
}
