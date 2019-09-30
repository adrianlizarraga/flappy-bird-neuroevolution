#pragma once
#include <memory>

class State;

class StateStackInterface {
public:
    virtual ~StateStackInterface() {}
    virtual void push(std::shared_ptr<State> state) = 0;
    virtual void pop() = 0;
    virtual void replaceTop(std::shared_ptr<State> state) = 0;
};