//
// Created by Daniel Bizari on 26/10/2020.
//

#include "EnemyMushroom.h"

EnemyMushroom::EnemyMushroom() = default;

void EnemyMushroom::init(int x, int y, std::string textureID, int currentFrame) {
    GameObject::init(x, y, textureID, currentFrame);
}

void EnemyMushroom::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {

}

EnemyMushroom::~EnemyMushroom() = default;
