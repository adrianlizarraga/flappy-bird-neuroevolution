#include <iostream>
#include <string>
#include "Game.h"

using namespace std;

int main(int argc, char* argv[]) {
    int mode = argc > 1 ? std::stoi(argv[1]) : 0;
    Game game(1200, 600, 120);

    game.loop(mode);

    return 0;
}