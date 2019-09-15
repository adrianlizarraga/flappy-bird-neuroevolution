#include "states/PlayState.h"
#include "AssetManager.h"
#include <chrono>

PlayState::PlayState(int width, int height)
    : m_width(width), m_height(height),
      m_ground(sf::FloatRect(0.0f, height - m_groundHeight, width, m_groundHeight)),
      m_background(sf::FloatRect(0.0f, 0.0f, width, m_backgroundHeight)),
      m_bird(200, 150, &m_ground, &m_background), m_randGapY(64, int(m_height - m_groundHeight - 64)),
      m_randGapHeight(100, 256), m_engine(std::chrono::system_clock::now().time_since_epoch().count()) {
    
    // Setup score text
    sf::Font &font = AssetManager::instance().getFont("data/flappy.ttf");
    m_statusLabel.setFont(font);
    m_statusLabel.setString("Score: 0");
    m_statusLabel.setCharacterSize(28);
    m_statusLabel.setFillColor(sf::Color(219, 111, 57));
    m_statusLabel.setOutlineThickness(1.0f);
    m_statusLabel.setStyle(sf::Text::Bold);
    m_statusLabel.setPosition(sf::Vector2f(width - 280, 0));

    this->reset();
}

PlayState::~PlayState() {

}

void PlayState::addPipe() {
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

void PlayState::cleanupPipes() {
    if (m_pipes.size() > 0) {
        auto firstPipeIterator = m_pipes.begin();
        sf::FloatRect firstPipeBbox = firstPipeIterator->getCombinedBoundingBox();

        if (firstPipeBbox.left + firstPipeBbox.width < 0.0f) {
            m_pipes.erase(firstPipeIterator);
        }
    }
}

void PlayState::reset() {
    m_pipes.clear();
    m_frame = 0;
    m_pipeNumber = 0;
    m_bird.reset(200, 150);
    m_statusLabel.setString("Score: 0");
}

void PlayState::handleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space) {
            m_bird.flap();
        } else if (event.key.code == sf::Keyboard::R) {
            reset();
        }
    }
}

void PlayState::update(float elapsed) {

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

void PlayState::draw(sf::RenderWindow& window) const {
    m_background.draw(window);
    m_bird.draw(window);
    m_ground.draw(window);

    for (auto &pipe : m_pipes) {
        pipe.draw(window);
    }

    window.draw(m_statusLabel);
}


