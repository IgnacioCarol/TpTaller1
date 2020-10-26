//
// Created by Daniel Bizari on 26/10/2020.
//

#include "Coin.h"

Coin::Coin() = default;

void Coin::init(int x, int y, std::string textureID, int currentFrame) {

    GameObject::init(x, y, textureID, currentFrame);
}

void Coin::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    //TODO preguntar lichano como mienrda hacer esto
}

Coin::~Coin() = default;
