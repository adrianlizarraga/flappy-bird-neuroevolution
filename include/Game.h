#pragma once
#include "Menu.h"
#include "states/StateStack.h"
#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include <random>
#include <vector>


class Game {
  private:
    int m_mode;
    int m_width, m_height;

    int m_frame = 0;
    bool m_paused = false;

    sf::RenderWindow m_window;
    sf::Clock m_clock;

    Menu m_menu;

    StateStack m_stateStack;

    void reset();
    void pollEvents();
    void update();
    void draw();

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