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


int main(int argc, char * argv[]) {

//#ifdef TEST
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//#endif
    Game* game = Game::Instance();

    if (!game->init("Level 1")) { //Aca inicializo el background
        Logger::getInstance() -> error("Error: the game could not be initialized");
        return 1;
    }
    if (!game->loadImages()){
        Logger::getInstance() -> error("Error: Loading the sprites went wrong");
        return 1;
    }
    if (!game-> loadTexts()) {
        Logger::getInstance()->error("Error: Loading texts went wrong");
        return 1;
    }
    game->createGameObjects();
    //Event handler
    SDL_Event e;

    while(game->isPlaying()){
        frameStart = SDL_GetTicks(); //To get the time at the start of the loop

        while(SDL_PollEvent(&e) != 0) {
            if (e.type  == SDL_QUIT ) {
                Game::Instance()->gameOver();
            }
        }

        game->handleEvents();
       // game->update();
        game->render();
        /*frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < DELAY_TIME){
            SDL_Delay((int)(DELAY_TIME - frameTime));
        }*/
        SDL_Delay(2);
    }
    Logger::getInstance() -> info("Game over\n");
    game->clean();
    delete game;

    SDL_Quit();
    return EXIT_SUCCESS;
}
