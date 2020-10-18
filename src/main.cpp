#include "Game.h"
#include <exception>

#include <iostream>

int main(int argc, char *args[]) {

    try {
        Game::Instance()->init();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    while(Game::Instance()->isRunning()) {
        Game::Instance()->handleUserEvents();
        Game::Instance()->updateState();
        Game::Instance()->render();
    }

    Game::Instance()->destroy();
    return 0;
}