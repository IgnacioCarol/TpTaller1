//
// Created by lisandro on 23/10/20.
//

#include "main.h"
#include "Game.h"
#include <SDL2/SDL.h>
#include <string>
#include "Mario.h"
#include "Hongo.h"

Game* game = NULL;

int main(int argc, char* argv[]){
    game = new Game();

    if (!game){
        printf("No se pudo crear el juego\n");
        return 1;
    }


    if (!game->init("Level 1", 800, 600)){ //Aca inicializo el background
        printf("No se pudo inicializar el juego\n");
        return 1;
    }

    if (!game->loadImages()){
        printf("Error cargando las imagenes\n");
        return 1;
    }
    Mario* marito = new Mario();
    Hongo* honguito = new Hongo();

    game->setGameObjects(marito, honguito);

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;
    /*int xValue = 0;
    int xValueBackground = 0;
    int yPosition = 450;
    int maxYPosition = 350;*/

    while(game->isPlaying()){

        while(SDL_PollEvent(&e) != 0) {
            if (e.type  == SDL_QUIT ) {
                printf("Entre a esto para cerrar\n");
                game->gameOver();
            }
        }

        game->handleEvents();
        //game->update();
        game->render();
        //SDL_Delay(10000);
        //printf("Termino el delay\n");
        SDL_Delay(2);
    }
    printf("Sali del juego");

    game->clean();

    return 0;
}
