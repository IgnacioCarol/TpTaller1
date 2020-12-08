#include "Enemy.h"
#include "../CharacterStates/CharacterState.h"


void Enemy::init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, CharacterState *state) {
    GameObject::init(x, y, std::move(textureID));
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

    if (inScene) {
        xPosition += direction;
        flipFlag = direction == enemyVelocity;
    }
}

void Enemy::draw(SDL_Renderer *renderer, int cameraX, int cameraY, size_t width, size_t height) {
    SDL_RendererFlip flip = (!flipFlag) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    enemyState -> draw(_textureID, xPosition - cameraX, yPosition - cameraY, width, height, renderer, flip);
}

Enemy::~Enemy() {
    delete this->enemyState;
}

void Enemy::setPosition(int x, int y) {
    xPosition = x;
    yPosition = y;
}

void Enemy::setDirection(int direction) {
    this -> direction = direction;
}

void Enemy::setState(std::string state) {
    GameObject::setState(state); //ToDo implementar esto para la prox fase
}


