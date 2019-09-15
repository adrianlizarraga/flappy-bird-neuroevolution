#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "states/StateStackInterface.h"

class State {
private:
    StateStackInterface& m_stateStack;
public:
    State(StateStackInterface& stateStack): m_stateStack(stateStack) {}
    
    virtual ~State() {}
    virtual void handleEvent(sf::Event event) = 0;
    virtual void update(float elapsed) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};