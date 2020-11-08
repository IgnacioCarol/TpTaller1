#include "PlatformSurprise.h"

PlatformSurprise::PlatformSurprise() = default;

void PlatformSurprise::init(int x, int y, std::string textureID) {
    GameObject::init(x, y, textureID);
    _currentFrame = 0;
}

void PlatformSurprise::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    _currentFrame = (delayCounter++ % sBlockDelay) ? _currentFrame : (++_currentFrame % sBlockFrames);
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, SBWidth, SBHeight,
                                          SBWidth * _currentFrame, renderer, SDL_FLIP_NONE);
    delayCounter++;
}

int PlatformSurprise::getHeight() {
    return SBHeight;
}
