#include <iostream>
#include "Game.h"

using namespace std;

int main() {
    Game game(1200, 600, 120);

    game.loop();

    return 0;
}