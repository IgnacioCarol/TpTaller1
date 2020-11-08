#include "EnemyTurtle.h"
#include "../GameMap.h"

void EnemyTurtle::init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, CharacterState *state) {

    x = GameMap::getInstance()->getRandomX(y);
    Enemy::init(x, y, textureID, camera, state);
}



void EnemyTurtle::draw(SDL_Renderer *renderer, int cameraX, int cameraY, size_t width, size_t height) {
    Enemy::draw(renderer, cameraX, cameraY, width, height);
}

void EnemyTurtle::walk() {
    Enemy::walk();
}

void EnemyTurtle::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    this->draw(renderer, cameraX, cameraY, etWidth, etHeight);
}