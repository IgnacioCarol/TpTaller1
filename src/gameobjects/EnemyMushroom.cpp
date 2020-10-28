//
// Created by Daniel Bizari on 26/10/2020.
//

#include "EnemyMushroom.h"
#include "../GameMap.h"

EnemyMushroom::EnemyMushroom() = default;

void EnemyMushroom::init(int x, int y, std::string textureID, int currentFrame) {
    y = 403; //ToDo Pasar a macro
    x = GameMap::getInstance()->getRandomX(y);
    GameObject::init(x, y, textureID, currentFrame);
}

void EnemyMushroom::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {

}

EnemyMushroom::~EnemyMushroom() = default;
