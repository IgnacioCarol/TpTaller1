#include "PlatformSurprise.h"

PlatformSurprise::PlatformSurprise() = default;

void PlatformSurprise::init(int x, int y, std::string fileName, std::string defaultImg, std::string textureID, int currentFrame) {
    GameObject::init(x, y, fileName, defaultImg, textureID, currentFrame);
}

void PlatformSurprise::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    _currentFrame = (delayCounter++ % sBlockDelay) ? _currentFrame : (++_currentFrame % sBlockFrames);
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, SBWidth, SBHeight,
                                          SBWidth * _currentFrame, renderer, SDL_FLIP_NONE);
    delayCounter++;
}

std::string PlatformSurprise::getFilePath() {
    return GameObject::getFilePath();
}

std::string PlatformSurprise::getID() {
    return GameObject::getID();
}

std::string PlatformSurprise::getDefault() {
    return GameObject::getDefault();
}
