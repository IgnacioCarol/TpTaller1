//
// Created by Daniel Bizari on 26/10/2020.
//

#include "PlatformNormal.h"

PlatformNormal::PlatformNormal() = default;

void PlatformNormal::init(int x, int y, std::string textureID) {
    GameObject::init(x, y, textureID);
}

void PlatformNormal::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, NORMAL_BLOCK_WIDTH, NORMAL_BLOCK_HEIGHT,
                                          0, renderer, SDL_FLIP_NONE);
}

int PlatformNormal::getHeight() {
    return NORMAL_BLOCK_HEIGHT;
}
