#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <deque>
#include "states/StateStackInterface.h"

class State;

class StateStack: public StateStackInterface {
private:
    std::deque<std::shared_ptr<State> > m_states;
public:
    StateStack();
    void push(std::shared_ptr<State> state);
    void pop();
    void replaceTop(std::shared_ptr<State> state);
    void handleEvent(sf::Event event);
    void update(float elapsed);
    void draw(sf::RenderWindow& window) const;
};
