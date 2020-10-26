//
// Created by Daniel Bizari on 26/10/2020.
//

#include "PlatformSurprise.h"

PlatformSurprise::PlatformSurprise() = default;

void PlatformSurprise::init(int x, int y, std::string textureID, int currentFrame) {
    GameObject::init(x, y, textureID, currentFrame);
}

void PlatformSurprise::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {

}

PlatformSurprise::~PlatformSurprise() = default;
