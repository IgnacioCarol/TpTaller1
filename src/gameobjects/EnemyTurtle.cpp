#include "EnemyTurtle.h"
#include "../GameMap.h"

void EnemyTurtle::init(size_t x, size_t y, std::string fileName, std::string defaultImg, std::string textureID, int currentFrame, SDL_Rect *camera, int framesAmount,
                       CharacterState *state) {

    x = GameMap::getInstance()->getRandomX(y);
    Enemy::init(x, y, fileName, defaultImg, textureID, currentFrame, camera, framesAmount, state);
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

std::string EnemyTurtle::getFilePath() {
    return Enemy::getFilePath();
}

std::string EnemyTurtle::getID() {
    return Enemy::getID();
}

std::string EnemyTurtle::getDefault() {
    return Enemy::getDefault();
}
