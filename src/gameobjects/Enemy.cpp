//
// Created by lisandro on 27/10/20.
//

#include "Enemy.h"


void Enemy::init(size_t x, size_t y, std::string textureID, int currentFrame, SDL_Rect *camera, int framesAmount,
                 CharacterState* state) {
    GameObject::init(x, y, std::move(textureID), currentFrame);
    enemyState = state;
}

void Enemy::move() {
    //enemyState -> move(this);
}

void Enemy::walk() {
    //Random movement
}

void Enemy::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    //Creo que aca no esta lo de la camara
    //enemyState -> draw(_textureID, xPosition, yPosition, pWidth, pHeight, renderer, flip);
}
