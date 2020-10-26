//
// Created by Daniel Bizari on 26/10/2020.
//

#include "Coin.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

Coin::Coin() = default;

void Coin::init(int x, int y, std::string textureID, int currentFrame) {
    srand (time(NULL));
    //TODO determinar si esa posicion esta libre, crear un gestor del mapa
    x = rand() % 30 + 10; //TODO averiguar rango de x del mapa, quiza esto depende del mapa asi que habria que ver si es necesario pasarle por parametro rango max
    GameObject::init(x, y, textureID, currentFrame);
}

void Coin::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    //TODO preguntar lichano como mienrda hacer esto
}

Coin::~Coin() = default;
