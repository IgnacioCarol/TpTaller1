//
// Created by Daniel Bizari on 26/10/2020.
//

#include "EnemyTurtle.h"
#include "../GameMap.h"

EnemyTurtle::EnemyTurtle() = default;

void EnemyTurtle::init(int x, int y, std::string textureID, int currentFrame) {
    y = 403; //ToDo Pasar a macro
    x = GameMap::getInstance()->getRandomX(y);
    GameObject::init(x, y, textureID, currentFrame);
}

void EnemyTurtle::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {

}

EnemyTurtle::~EnemyTurtle() = default;
