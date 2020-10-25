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
//    testing::InitGoogleTest(&argc, argv); //TODO agregar macro para correr tests solo en ambientes de test
//    return RUN_ALL_TESTS();
//#endif
        Game* game = Game::Instance();

        if (!game->init("Level 1", SCREEN_WIDTH, SCREEN_HEIGHT)){ //Aca inicializo el background
            printf("No se pudo inicializar el juego\n");
            return 1;    }

    if (!game->loadImages()){
        printf("Error cargando las imagenes\n");
        return 1;
    }

    game->createGameObjects();

    bool quit = false;

    //Event handler
    SDL_Event e;

    while(game->isPlaying()){

        while(SDL_PollEvent(&e) != 0) {
            if (e.type  == SDL_QUIT ) {
                printf("Entre a esto para cerrar\n");
                Game::Instance()->gameOver();
            }
        }

        game->handleEvents();
        //game->update();
        game->render();
        SDL_Delay(2);
    }
    printf("Sali del juego");
    game->clean();
    SDL_Quit();
    return EXIT_SUCCESS;
}
