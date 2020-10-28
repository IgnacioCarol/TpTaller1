//
// Created by nacho on 10/10/20.
//
//
//  main.cpp
//  test
//
//  Created by Daniel Bizari on 08/10/2020.
//  Copyright © 2020 Daniel Bizari. All rights reserved.
//
#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "logger/logger.h"
#include "Game.h"
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char * argv[]) {

//#ifdef TEST
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//#endif
    Game* game = Game::Instance();

    if (!game->init("Level 1", SCREEN_WIDTH, SCREEN_HEIGHT)) { //Aca inicializo el background
        Logger::getInstance() -> error("Error: the game could not be initialized");
        return 1;
    }

    if (!game->loadImages()){
        Logger::getInstance() -> error("Error: Loading the sprites went wrong");
        return 1;
    }

    if (!game->loadLevel(1)) {
        Logger::getInstance()->error("Error: Loading level went wrong");
        return 1;
    }

    game->createGameObjects();

    bool quit = false;

    //Event handler
    SDL_Event e;

    while(game->isPlaying()){

        while(SDL_PollEvent(&e) != 0) {
            if (e.type  == SDL_QUIT ) {
                Game::Instance()->gameOver();
            }
        }

        game->handleEvents();
        //game->update();
        game->render();
        SDL_Delay(2);
    }
    Logger::getInstance() -> info("Game over");
    game->clean();
    SDL_Quit();
    return EXIT_SUCCESS;
}
