//
// Created by Daniel Bizari on 26/10/2020.
//

#include "EnemyTurtle.h"

EnemyTurtle::EnemyTurtle() = default;

void EnemyTurtle::init(int x, int y, std::string textureID, int currentFrame) {
    GameObject::init(x, y, textureID, currentFrame);
}

void EnemyTurtle::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {

}

EnemyTurtle::~EnemyTurtle() = default;
