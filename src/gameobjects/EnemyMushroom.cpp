#include "../CharacterStates/EnemyMovement.h"
#include "EnemyMushroom.h"
#include "../GameMap.h"

void
EnemyMushroom::init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, CharacterState *state) {

    x = GameMap::getInstance()->getRandomX(y);
    Enemy::init(x, y, textureID, camera, state);
    type = GOT_ENEMY_MUSHROOM;
    points = 100;
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
    this->draw(renderer, cameraX, cameraY, ENEMY_WIDTH, ENEMY_HEIGHT);
}