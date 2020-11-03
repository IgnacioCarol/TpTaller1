//
// Created by Daniel Bizari on 26/10/2020.
//

#include "PlatformSurprise.h"

PlatformSurprise::PlatformSurprise() = default;

void PlatformSurprise::init(int x, int y, std::string textureID, int currentFrame) {
    GameObject::init(x, y, textureID, currentFrame);
}

void PlatformSurprise::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    _currentFrame = (delayCounter++ % sBlockDelay) ? _currentFrame : ++_currentFrame;
    _currentFrame = (_currentFrame % sBlockFrames);
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, SBWidth, SBHeight,
                                          SBWidth * _currentFrame, renderer, SDL_FLIP_NONE);
    delayCounter++;
}