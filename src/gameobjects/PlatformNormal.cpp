//
// Created by Daniel Bizari on 26/10/2020.
//

#include "PlatformNormal.h"

PlatformNormal::PlatformNormal() = default;

void PlatformNormal::init(int x, int y, std::string textureID, int currentFrame) {
    GameObject::init(x, y, textureID, currentFrame);
}

void PlatformNormal::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {

}

PlatformNormal::~PlatformNormal() = default;
