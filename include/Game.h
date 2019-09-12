#pragma once
#include "AssetManager.h"
#include "Background.h"
#include "Bird.h"
#include "Ground.h"
#include "Menu.h"
#include "Pipe.h"
#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include <random>
#include <vector>


class Game {
  private:
    int m_mode;
    int m_width, m_height;
    float m_groundHeight = 128.0f;
    float m_backgroundHeight = 472.0f;

    std::default_random_engine m_engine;
    std::uniform_int_distribution<int> m_randGapY;
    std::uniform_int_distribution<int> m_randGapHeight;
    std::uniform_int_distribution<int> m_randPercentage;

    int m_frame = 0;
    int m_pipeNumber = 0;
    bool m_paused = false;

    sf::RenderWindow m_window;
    sf::Clock m_clock;

    AssetManager m_assetManager;

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

    Menu m_menu;

    void addPipe();
    void cleanupPipes();
    void reset();
    void pollEvents();
    void updatePlayer(float elapsed);
    void updateTraining(float elapsed);
    void updateAI(float elapsed);
    void draw();

    int selectBird();

  public:
    Game(int width, int height, int fps = 120, int mode = 0);
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    void loop();
    bool getPaused() const;
    void setPaused(bool paused);
    void setMode(int mode);
    int getMode() const;
};