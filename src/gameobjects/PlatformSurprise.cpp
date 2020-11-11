#include "PlatformSurprise.h"

PlatformSurprise::PlatformSurprise() = default;

void PlatformSurprise::init(int x, int y, std::string textureID) {
    GameObject::init(x, y, textureID);
    _currentFrame = 0;
}

void PlatformSurprise::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    _currentFrame = (delayCounter++ % SURPRISE_BLOCK_DELAY) ? _currentFrame : (++_currentFrame % SURPRISE_BLOCK_FRAMES);
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, SURPRISE_BLOCK_WIDTH, SURPRISE_BLOCK_HEIGHT,
                                          SURPRISE_BLOCK_WIDTH * _currentFrame, renderer, SDL_FLIP_NONE);
    delayCounter++;
}

int PlatformSurprise::getHeight() {
    return SURPRISE_BLOCK_HEIGHT;
}
