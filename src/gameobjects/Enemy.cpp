//
// Created by lisandro on 27/10/20.
//

#include "Enemy.h"
#include "../CharacterStates/CharacterState.h"
#include "../CharacterStates/EnemyMovement.h"


void Enemy::init(size_t x, size_t y, std::string textureID, int currentFrame, SDL_Rect *camera, int framesAmount,
                 CharacterState* state) {
    GameObject::init(x, y, std::move(textureID), currentFrame);
    enemyState = state;
    cam = camera;
}

void Enemy::move() {
    enemyState -> move(this);
}

void Enemy::walk() {
    int camXPosition = cam -> x;
    if (xPosition >= camXPosition && xPosition <= camXPosition + 800){ //The enemy is on scene
        inScene = true;
    }
    else if (inScene){ //The enemy was in scene but its over
        inScene = false;
        //TODO decidir que hacer con el enemigo una vez que lo pasamos con la camara. Si vai fora dejar este if, sino volarlo
    }
    if (inScene) {
        xPosition = ((xPosition += direction) < 0) ? 0 : xPosition;

        if (xPosition == camXPosition){
            direction = 1;
        }
        else if (xPosition == camXPosition + 800){
            direction = -1;
        }
        flipFlag = direction == 1;
    }
}

void Enemy::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    SDL_RendererFlip flip = (flipFlag) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    enemyState -> draw(_textureID, xPosition - cameraX, yPosition - cameraY, EnemyWidth, EnemyHeight, renderer, flip);
}

Enemy::Enemy() {
    this -> init(5, 403, "goomba", 0, NULL, 3, new EnemyMovement(0, 3));
}
