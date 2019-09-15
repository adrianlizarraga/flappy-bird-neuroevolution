#include "states/StateStack.h"
#include "states/State.h"

StateStack::StateStack() {

}

void StateStack::push(std::shared_ptr<State> state) {
    this->m_states.push_back(state);
}

void StateStack::pop() {
    this->m_states.pop_back();
}

void StateStack::handleEvent(sf::Event event) {
    
    // Only the state at the top of the stack should handle events.
    if (!this->m_states.empty()) {
        this->m_states.back()->handleEvent(event);
    }
}

void StateStack::update(float elapsed) {

    // Only the state at the top of the stack should be updated.
    if (!this->m_states.empty()) {
        this->m_states.back()->update(elapsed);
    }
}

void StateStack::draw(sf::RenderWindow& window) const {

    // Draw all states in the stack.
    for (auto state : this->m_states) {
        state->draw(window);
    }
}