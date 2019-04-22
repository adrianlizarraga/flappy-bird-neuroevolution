#include <iostream>
#include <string>
#include "Game.h"

using namespace std;

int main(int argc, char* argv[]) {
    std::string mode = argc > 1 ? argv[1] : "train";
    Game game(1200, 600, 120);

    game.loop();

    return 0;
}