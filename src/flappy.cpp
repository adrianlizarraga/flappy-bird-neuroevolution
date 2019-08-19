#include <iostream>
#include <string>
#include "Game.h"

using namespace std;

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int mode = argc > 1 ? std::stoi(argv[1]) : 0;
    Game game(1200, 600, 120, mode);

    game.loop();

    return 0;
}