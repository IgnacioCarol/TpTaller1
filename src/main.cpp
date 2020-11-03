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
#define MAX_ARGS 2

bool parseCLI(int argc, char * argv[], std::string * xmlPath) {
    if (argc > MAX_ARGS) {
        return false;
    } else if (argc == 1) {
        *xmlPath = "./resources/config.xml"; // set default xml
    } else {
        *xmlPath = argv[1];
    }

    return true;
}

int main(int argc, char * argv[]) {

//#ifdef TEST
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//#endif
    Logger::getInstance()->info("System initializing...");

    std::string xmlPath = "";

    if (!parseCLI(argc, argv, &xmlPath)) {
        Logger::getInstance() -> error("Error: incorrect the program does not support the amount of CLI params");
        return EXIT_FAILURE;
    }

    Logger::getInstance()->info("XML Path: " + xmlPath);

    Game* game = Game::Instance();

    if (!game->init("Level 1", SCREEN_WIDTH, SCREEN_HEIGHT, xmlPath)){ //Aca inicializo el background
        Logger::getInstance() -> error("Error: the game could not be initialized");
        return EXIT_FAILURE;
    }

    if (!game->loadImages()){
        Logger::getInstance() -> error("Error: Loading the sprites went wrong");
        return EXIT_FAILURE;
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

    game->clean();
    Logger::getInstance() -> info("Game over");

    SDL_Quit();
    return EXIT_SUCCESS;
}
