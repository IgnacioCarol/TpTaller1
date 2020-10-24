#include "Game.h"
#include <exception>

#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "logger/logger.h"

int main(int argc, char * argv[]) {

#ifdef TEST
    testing::InitGoogleTest(&argc, argv); //TODO agregar macro para correr tests solo en ambientes de test
    return RUN_ALL_TESTS();
#endif

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
