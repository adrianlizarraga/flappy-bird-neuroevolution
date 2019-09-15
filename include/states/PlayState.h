#pragma once
#include "Background.h"
#include "Bird.h"
#include "Ground.h"
#include "Pipe.h"
#include "states/State.h"
#include <SFML/Graphics.hpp>
#include <list>

class PlayState: public State {
private:
    int m_width, m_height;
    float m_groundHeight = 128.0f;
    float m_backgroundHeight = 472.0f;

    std::default_random_engine m_engine;
    std::uniform_int_distribution<int> m_randGapY;
    std::uniform_int_distribution<int> m_randGapHeight;

    int m_frame = 0; // TODO: replace with time that is incremented on update and reset after adding a pipe
    int m_pipeNumber = 0;
    bool m_paused = false;

    sf::Text m_statusLabel;
    Ground m_ground;
    Background m_background;
    Bird m_bird;
    std::list<PipePair> m_pipes;

    void addPipe();
    void cleanupPipes();
    void reset();
public:
    PlayState(int width, int height, StateStackInterface& stateStack);
    ~PlayState();

    void handleEvent(sf::Event event);
    void update(float elapsed);
    void draw(sf::RenderWindow& window) const;
};