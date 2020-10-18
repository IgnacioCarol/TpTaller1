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
#include "logger.h"

int main(int argc, char * argv[]) {

#ifdef TEST
    testing::InitGoogleTest(&argc, argv); //TODO agregar macro para correr tests solo en ambientes de test
    return RUN_ALL_TESTS();
#endif

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error inicializando: " << SDL_GetError() << std::endl;
    }

    SDL_Window * window = SDL_CreateWindow("holi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_ALLOW_HIGHDPI);
    if(!window) {
        std::cout << "Error creando window" << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Event windowEvent;
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(3000);

    while(true) {
        if (SDL_PollEvent(&windowEvent) && windowEvent.type == SDL_QUIT) {
            break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
