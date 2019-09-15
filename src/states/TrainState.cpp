#include "states/TrainState.h"
#include "AssetManager.h"
#include <chrono>

TrainState::TrainState(int width, int height, StateStackInterface& stateStack)
    : State(stateStack), m_width(width), m_height(height),
      m_ground(sf::FloatRect(0.0f, height - m_groundHeight, width, m_groundHeight)),
      m_background(sf::FloatRect(0.0f, 0.0f, width, m_backgroundHeight)),
      m_bird(200, 150, &m_ground, &m_background), m_randGapY(64, int(m_height - m_groundHeight - 64)),
      m_randGapHeight(100, 256), m_randPercentage(0, 100), m_engine(std::chrono::system_clock::now().time_since_epoch().count()),
      m_trainingBirdFitnesses(m_populationSize, 0.f) {
    
    // Setup score text
    sf::Font &font = AssetManager::instance().getFont("data/flappy.ttf");
    m_statusLabel.setFont(font);
    m_statusLabel.setString("Score: 0");
    m_statusLabel.setCharacterSize(28);
    m_statusLabel.setFillColor(sf::Color(219, 111, 57));
    m_statusLabel.setOutlineThickness(1.0f);
    m_statusLabel.setStyle(sf::Text::Bold);
    m_statusLabel.setPosition(sf::Vector2f(width - 320, 0));

    this->reset();
}

TrainState::~TrainState() {

}

void TrainState::addPipe() {
    const float pipeHeight = m_height - m_groundHeight;

    // Add a new pipe every few frames
    if (m_frame % 400 == 0) {
        float pipeWidth = 96.f;
        float gapHeight = m_randGapHeight(m_engine);
        float gapY = std::min(m_randGapY(m_engine), int(m_height - (m_groundHeight + gapHeight + 64)));

        m_pipes.push_back(PipePair(m_pipeNumber, sf::FloatRect(m_width, 0.0f, pipeWidth, pipeHeight), gapY, gapHeight));

        m_pipeNumber++;
    }
}

void TrainState::cleanupPipes() {
    if (m_pipes.size() > 0) {
        auto firstPipeIterator = m_pipes.begin();
        sf::FloatRect firstPipeBbox = firstPipeIterator->getCombinedBoundingBox();

        if (firstPipeBbox.left + firstPipeBbox.width < 0.0f) {
            m_pipes.erase(firstPipeIterator);
        }
    }
}

void TrainState::reset() {
    m_pipes.clear();
    m_frame = 0;
    m_pipeNumber = 0;
    m_generation = 0;
    m_bestScore = 0;
    m_deadTrainingBirds.clear();
    m_trainingBirds.clear();
            
    for (int i = 0; i < m_populationSize; ++i) {
        m_trainingBirds.push_back(std::make_shared<Bird>(200, 150, &m_ground, &m_background));
        m_trainingBirdFitnesses[i] = 0.f;
    }

    m_statusLabel.setString("Generation 0\nBest fitness 0%");
}

int TrainState::selectBird() {

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

void TrainState::handleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::R) {
            reset();
        }
    }
}

void TrainState::update(float elapsed) {
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
            m_trainingBirds.push_back(std::make_shared<Bird>(200, 150, &m_ground, &m_background, brain));
        }

        // Always keep a clone of the best bird in the previous generation.
        alai::MLPNetwork brain = m_deadTrainingBirds[maxFitnessIndex]->getBrain();

        brain.mutate(0.05, 0.001);
        m_trainingBirds.push_back(std::make_shared<Bird>(200, 150, &m_ground, &m_background, brain));

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

void TrainState::draw(sf::RenderWindow& window) const {
    m_background.draw(window);
    m_ground.draw(window);

    // Draw any training birds that are still alive
    for (auto &bird : m_trainingBirds) {
        bird->draw(window);
    }

    for (auto &pipe : m_pipes) {
        pipe.draw(window);
    }

    window.draw(m_statusLabel);
}


