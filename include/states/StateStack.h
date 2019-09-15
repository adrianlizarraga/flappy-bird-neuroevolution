#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <deque>
#include "states/State.h"

class StateStack {
private:
    std::deque<std::shared_ptr<State> > m_states;
public:
    StateStack();
    void push(std::shared_ptr<State> state);
    void pop();
    void handleEvent(sf::Event event);
    void update(float elapsed);
    void draw(sf::RenderWindow& window) const;
};