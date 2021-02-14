#include "EnemyTurtle.h"
#include "../GameMap.h"

void EnemyTurtle::init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, CharacterState *state) {

    x = GameMap::getInstance()->getRandomX(y);
    Enemy::init(x, y, textureID, camera, state);
    type = GOT_ENEMY_TURTLE;
    pointsToPlayer = POINTS_TURTLE;
    floor = 435;
}



void EnemyTurtle::draw(SDL_Renderer *renderer, int cameraX, int cameraY, size_t width, size_t height) {
    Enemy::draw(renderer, cameraX, cameraY, width, height);
}

void EnemyTurtle::walk() {
    Enemy::walk();
}

void EnemyTurtle::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    this->draw(renderer, cameraX, cameraY, ENEMY_WIDTH, ENEMY_HEIGHT);
}

int EnemyTurtle::getFloorPosition() {
    return -55;
}