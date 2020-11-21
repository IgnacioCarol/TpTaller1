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

#define FPS 40;
const int DELAY_TIME = 1000.0f / FPS;
Uint32 frameStart, frameTime;
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

    if (!game->init("Level 1")) { //Aca inicializo el background
        Logger::getInstance() -> error("Error: the game could not be initialized");
        return 1;
    }

    game->createGameObjects(); //ToDo refactorizar y mover al Factory, factory tiene que ser el unico responsable de instanciar gameObjects

    if (!game->loadImages()){
        Logger::getInstance() -> error("Error: Loading the sprites went wrong");
        return EXIT_FAILURE;
    }
    if (!game-> loadTexts()) {
        Logger::getInstance()->error("Error: Loading texts went wrong");
        return 1;
    }

    //Event handler
    SDL_Event e;

    while(game->isPlaying()){

        while(SDL_PollEvent(&e) != 0) {
            if (e.type  == SDL_QUIT ) {
                Game::Instance()->gameOver();
            }
        }

        game->handleEvents();
       // game->update();
        game->render();
        SDL_Delay(4);
    }
    Logger::getInstance() -> info("Game over");
    game->clean();
    delete game;
    delete Logger::getInstance();

    SDL_Quit();
    return EXIT_SUCCESS;
}
