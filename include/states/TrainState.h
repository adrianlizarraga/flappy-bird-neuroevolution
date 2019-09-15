#pragma once
#include "entity/Background.h"
#include "entity/Bird.h"
#include "entity/Ground.h"
#include "entity/Pipe.h"
#include "states/State.h"
#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include <vector>

class TrainState: public State {
private:
    int m_width, m_height;
    float m_groundHeight = 128.0f;
    float m_backgroundHeight = 472.0f;

    std::default_random_engine m_engine;
    std::uniform_int_distribution<int> m_randGapY;
    std::uniform_int_distribution<int> m_randGapHeight;
    std::uniform_int_distribution<int> m_randPercentage;

    int m_frame = 0; // TODO: replace with time that is incremented on update and reset after adding a pipe
    int m_pipeNumber = 0;
    bool m_paused = false;

    sf::Text m_statusLabel;
    Ground m_ground;
    Background m_background;
    Bird m_bird;
    std::list<PipePair> m_pipes;

    int m_populationSize = 500;
    int m_generation = 0;
    int m_bestScore = 0;
    std::vector<std::shared_ptr<Bird> > m_trainingBirds;
    std::vector<std::shared_ptr<Bird> > m_deadTrainingBirds;
    std::vector<double> m_trainingBirdFitnesses;

    void addPipe();
    void cleanupPipes();
    void reset();

    int selectBird();
public:
    TrainState(int width, int height, StateStackInterface& stateStack);
    ~TrainState();

    void handleEvent(sf::Event event);
    void update(float elapsed);
    void draw(sf::RenderWindow& window) const;
};