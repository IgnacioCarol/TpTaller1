#include "../CharacterStates/EnemyMovement.h"
#include "EnemyMushroom.h"

void
EnemyMushroom::init(size_t x, size_t y, std::string textureID, int currentFrame, SDL_Rect *camera, int framesAmount,
                    CharacterState *state) {
    Enemy::init(x, y, textureID, currentFrame, camera, framesAmount, state);
}

void EnemyMushroom::move() {
    Enemy::move();
}

void EnemyMushroom::walk() {
    Enemy::walk();
}

void EnemyMushroom::draw(SDL_Renderer *renderer, int cameraX, int cameraY, size_t width, size_t height) {
    Enemy::draw(renderer, cameraX, cameraY, width, height);
}

void EnemyMushroom::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    this->draw(renderer, cameraY, cameraY, emWidth, emHeight);
}
