#include "Game.h"
#include <chrono>
#include <algorithm>
#include <iostream>

Game::Game(int width, int height, int fps, int mode)
    : m_width(width), m_height(height), m_window(sf::VideoMode(width, height), "Flappy bird: live, die, and repeat"), m_mode(mode),
      m_ground(sf::FloatRect(0.0f, height - m_groundHeight, width, m_groundHeight), m_assetManager),
      m_background(sf::FloatRect(0.0f, 0.0f, width, m_backgroundHeight), m_assetManager),
      m_bird(200, 150, m_assetManager, &m_ground, &m_background), m_randGapY(64, int(m_height - m_groundHeight - 64)),
      m_randGapHeight(100, 256), m_randPercentage(0, 100), m_engine(std::chrono::system_clock::now().time_since_epoch().count()),
      m_trainingBirdFitnesses(m_populationSize, 0.f),
      m_menu(this, sf::FloatRect(width / 2.f - width / 4.f, height / 2.f - height / 4.f, width / 2.f, height / 2.f)) {

    // Setup score text
    sf::Font &font = m_assetManager.getFont("data/trench.ttf");
    m_statusLabel.setFont(font);
    m_statusLabel.setString("Score: 0");
    m_statusLabel.setCharacterSize(28);
    m_statusLabel.setFillColor(sf::Color(219, 111, 57));
    m_statusLabel.setOutlineThickness(1.0f);
    m_statusLabel.setStyle(sf::Text::Bold);
    m_statusLabel.setPosition(sf::Vector2f(width - 280, 0));

    m_window.setFramerateLimit(fps);

    this->reset();
}

void Game::addPipe() {
    const float pipeHeight = m_height - m_groundHeight;

    // Add a new pipe every few frames
    if (m_frame % 400 == 0) {
        float pipeWidth = 96.f;
        float gapHeight = m_randGapHeight(m_engine);
        float gapY = std::min(m_randGapY(m_engine), int(m_height - (m_groundHeight + gapHeight + 64)));

        m_pipes.push_back(PipePair(m_pipeNumber, sf::FloatRect(m_width, 0.0f, pipeWidth, pipeHeight), gapY, gapHeight, m_assetManager));

        m_pipeNumber++;
    }
}

void Game::cleanupPipes() {
    if (m_pipes.size() > 0) {
        auto firstPipeIterator = m_pipes.begin();
        sf::FloatRect firstPipeBbox = firstPipeIterator->getCombinedBoundingBox();

        if (firstPipeBbox.left + firstPipeBbox.width < 0.0f) {
            m_pipes.erase(firstPipeIterator);
        }
    }
}

void Game::reset() {
    m_pipes.clear();
    m_frame = 0;
    m_pipeNumber = 0;

    if (m_mode == 0 || m_mode == 2) {
        m_bird.reset(200, 150);
        m_statusLabel.setString("Score: 0");
    } else if (m_mode == 1) {
        m_generation = 0;
        m_bestScore = 0;
        m_deadTrainingBirds.clear();
        m_trainingBirds.clear();
            
        for (int i = 0; i < m_populationSize; ++i) {
            m_trainingBirds.push_back(std::make_shared<Bird>(200, 150, m_assetManager, &m_ground, &m_background));
            m_trainingBirdFitnesses[i] = 0.f;
        }

        m_statusLabel.setString("Generation 0\nBest fitness 0%");
    }
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
        if (event.type == sf::Event::Closed) {
            m_window.close();
        } else if (m_paused) {
            m_menu.handleEvent(event);
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space && m_mode == 0) {
                m_bird.flap();
            } else if (event.key.code == sf::Keyboard::R) {
                reset();
            } else if (event.key.code == sf::Keyboard::P) {
                this->setPaused(!m_paused);
            } else if (event.key.code == sf::Keyboard::Escape) {
                m_window.close();
            }
        }
    }
}

void Game::draw() {
    m_window.clear(sf::Color::White);
    m_background.draw(m_window);

    if (m_mode == 0 || m_mode == 2) {
        m_bird.draw(m_window);
    } else {

        // Draw any training birds that are still alive
        for (auto &bird : m_trainingBirds) {
            bird->draw(m_window);
        }
    }

    m_ground.draw(m_window);

    for (auto &pipe : m_pipes) {
        pipe.draw(m_window);
    }

    m_window.draw(m_statusLabel);

    if (m_paused) {
        m_menu.draw(m_window);
    }

    m_window.display();
}

void Game::loop() {
    while (m_window.isOpen()) {
        this->pollEvents();

        float elapsed = m_clock.restart().asSeconds();

        if (!m_paused) {
            if (m_mode == 0) {
                this->updatePlayer(elapsed);
            } else if (m_mode == 1) {
                this->updateTraining(elapsed);
            } else if (m_mode == 2) {
                this->updateAI(elapsed);
            }
        } else {
            m_menu.update(elapsed);
        }

        // Draw
        this->draw();
    }
}

void Game::updatePlayer(float elapsed) {

    // Add a pipe if necessary
    this->addPipe();

    // Update bird physics
    m_bird.update(elapsed);

    // Update pipe physics, check bird/pipe collision, and update score.
    bool collided = false;
    for (auto &pipe : m_pipes) {
        pipe.update(elapsed);

        bool intersects = m_bird.checkPipeCollision(pipe);
        collided = collided || intersects;

        m_statusLabel.setString("Score: " + std::to_string(m_bird.getScore()));
    }

    // Clean up
    this->cleanupPipes();

    // Reset game if collided
    if (collided) {
        this->reset();
    } else {
        m_frame++;
    }
}

void Game::updateTraining(float elapsed) {
    this->addPipe();

    // Update pipe physics
    for (auto &pipe : m_pipes) {
        pipe.update(elapsed);
    }

    // Update birds and check collisions
    for (auto it = m_trainingBirds.begin(); it != m_trainingBirds.end();) {
        (*it)->update(elapsed);

        bool collided = false;
        for (auto &pipe : m_pipes) {
            bool intersects = (*it)->checkPipeCollision(pipe);
            collided = collided || intersects;
        }

        bool stopped = (*it)->intersects(m_ground) && std::abs((*it)->getVelocity().y) < 2;

        if (collided || stopped) {
            m_deadTrainingBirds.push_back(*it);
            it = m_trainingBirds.erase(it);
        }
        else {
            (*it)->sense(m_pipes, m_width, m_height);
            ++it;
        }
    }

    // Clean up
    this->cleanupPipes();

    // Check if any birds are still alive.
    if (!m_trainingBirds.empty()) {
        m_frame++;
    } else {

        // Calculate fitness by normalizing individual bird scores.
        int totalScore = std::accumulate(m_deadTrainingBirds.begin(), m_deadTrainingBirds.end(), 0, [](int acc, const std::shared_ptr<Bird>& bird) {
            return acc + bird->getScore();
        });

        double maxFitness = 0.0;
        int maxFitnessIndex = 0;

        for (int i = 0; i < m_deadTrainingBirds.size(); ++i) {
            m_trainingBirdFitnesses[i] = totalScore > 0 ? (m_deadTrainingBirds[i]->getScore()) / (float) totalScore : 0.f;

            if (m_trainingBirdFitnesses[i] >= maxFitness) {
                maxFitness = m_trainingBirdFitnesses[i];
                maxFitnessIndex = i;
            }
        }

        // Select birds with probably proportional to fitness and clone (w/ mutation)
        for (int i = 0; i < m_populationSize - 1; ++i) {
            int birdIndex = this->selectBird();
            alai::MLPNetwork brain = m_deadTrainingBirds[birdIndex]->getBrain();

            brain.mutate(0.1, 0.05);
            m_trainingBirds.push_back(std::make_shared<Bird>(200, 150, m_assetManager, &m_ground, &m_background, brain));
        }

        // Always keep a clone of the best bird in the previous generation.
        alai::MLPNetwork brain = m_deadTrainingBirds[maxFitnessIndex]->getBrain();

        brain.mutate(0.05, 0.001);
        m_trainingBirds.push_back(std::make_shared<Bird>(200, 150, m_assetManager, &m_ground, &m_background, brain));

        // Reset state for next generation
        m_generation++;
        m_bestScore = m_deadTrainingBirds[maxFitnessIndex]->getScore();
        m_pipes.clear();
        m_frame = 0;
        m_pipeNumber = 0;
        m_deadTrainingBirds.clear();
    }

    m_statusLabel.setString("Generation: " + std::to_string(m_generation) + "\nBest score: " + std::to_string(m_bestScore) + "\nBirds remaining: " + std::to_string(m_trainingBirds.size()));
}

int Game::selectBird() {

    // Generate a new population by cloning birds. Select birds to clone with probability proportional to fitness.
    double randNum = m_randPercentage(m_engine);
    int selectedBirdIndex = 0;

    for (int i = 0; i < m_trainingBirdFitnesses.size(); ++i) {
        randNum -= m_trainingBirdFitnesses[i] * 100;
        selectedBirdIndex = i;

        if (randNum <= 0) {
            break;
        }
    }

    return selectedBirdIndex;
}

void Game::updateAI(float elapsed) {

    // Add a pipe if necessary.
    this->addPipe();

    // Update bird physics
    m_bird.update(elapsed);

    // Update pipe physics, check bird/pipe collision, and update score.
    bool collided = false;
    for (auto &pipe : m_pipes) {
        pipe.update(elapsed);

        bool intersects = m_bird.checkPipeCollision(pipe);
        collided = collided || intersects;

        m_statusLabel.setString("Score: " + std::to_string(m_bird.getScore()));
    }

    // Clean up
    this->cleanupPipes();

    // Reset game if collided
    if (collided) {
        this->reset();
    } else {
        m_frame++;
    }

    // Let bird do its own thing with its neural network brain.
    m_bird.sense(m_pipes, m_width, m_height);
}