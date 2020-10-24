//
// Created by lisandro on 23/10/20.
//

#include "Game.h"

bool Game::init(const char *levelName, int width, int height) {
    //SDL initializing
    if (!SDL_Init(SDL_INIT_EVERYTHING)){
        printf("SDL init success\n");
        window = SDL_CreateWindow(levelName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width, height, 0);
        if (window){
            printf("Window init success\n");
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer){
                printf("Renderer init success\n");
            }

            else {
                printf("Render init fail\n");
                return false;
            }
        }
        else{
            printf("Window init fail\n");
            return false;
        }
    }
    else{
        printf("SDL init fail\n");
        return false;
    }

    printf("Init success\n");
    playing = true;
    return true;
}

void Game::render() {
    SDL_RenderClear(renderer);
    textureManager->drawBackground(800, 600, renderer);
    //textureManager->draw("dino", mario->position()[0], mario->position()[1], 1, 1, renderer);
    mario->draw(renderer);
    //Aca deberia ir la parte de cargar las imagenes y esas cosas
    //El textureManager deberia tener un puntero al renderer asi le carga las cosas
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    printf("Cleaning game");
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Game::handleEvents() {
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
    {
        printf("Entre a correr\n");
        printf("Posicion de Marito en X: %d \n", mario->getXPosition());
        mario->run(1);
    }
    if(currentKeyStates[ SDL_SCANCODE_LEFT ])
    {
        printf("Entre a correr a la izquierda\n");
        mario->run(-1);
    }
    if (currentKeyStates [ SDL_SCANCODE_UP] && !mario->isJumping() && mario->finishJump()) {
        printf("Entre a saltar\n");
        mario->jump();
    }
    //hasta aca terminaron las acciones, si no aprieto nada hago que baje el culiado
    /*else if (!mario->finishJump() ){
        mario->descender();
    }*/

    else{
        mario->setMarioState("dino");
    }

}

bool Game::loadImages() {
    bool success;
    success = textureManager->load("../Sprites/sprites_prueba/dino.png", "dino", renderer);
    if (!success){
        printf("No encontre la ruta\n");
        return false;
    }
    success = success && textureManager -> load("../Sprites/sprites_prueba/backgroundCompleto.png", "BG", renderer);
    success = success && textureManager -> load("../Sprites/sprites_prueba/dog.png", "dog", renderer);
    success = success && textureManager -> load("../Sprites/sprites_prueba/RunDog.png", "runDog", renderer);
    return success;
}