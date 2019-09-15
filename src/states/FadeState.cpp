#include "states/FadeState.h"
#include "asset/AssetManager.h"
#include <cmath>

FadeState::FadeState(int width, int height, StateStackInterface& stateStack, 
                     sf::Color fadeColor, int initOpacity, int finalOpacity, float fadeDuration,
                     std::function<void()> fadeCompleteCallback)
    : State(stateStack), m_width(width), m_height(height), m_rectangle(sf::Vector2f(width, height)),
      m_fadeColor(fadeColor), m_initOpacity(initOpacity), m_finalOpacity(finalOpacity), m_fadeDuration(fadeDuration),
      m_fadeCompleteCallback(fadeCompleteCallback) {

    m_rectangle.setPosition(0.f, 0.f);
    m_rectangle.setFillColor(sf::Color(fadeColor.r, fadeColor.g, fadeColor.b, initOpacity));
}

FadeState::~FadeState() {}

void FadeState::handleEvent(sf::Event event) {
    // Do nothing
}

void FadeState::update(float elapsed) {
    float percent = m_timeElapsed / m_fadeDuration;

    if (percent <= 1.f) {
        // Fade in or out
        int opacity = (int)((1.f - percent) * m_initOpacity + percent * m_finalOpacity);

        m_rectangle.setFillColor(sf::Color(m_fadeColor.r, m_fadeColor.g, m_fadeColor.b, opacity));

        m_timeElapsed = m_timeElapsed + elapsed;
    }
    else {
        m_stateStack.pop();
        m_fadeCompleteCallback();
    }
}

void FadeState::draw(sf::RenderWindow& window) const {
    window.draw(m_rectangle);
}