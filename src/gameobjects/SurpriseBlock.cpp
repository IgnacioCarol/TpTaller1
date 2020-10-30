//
// Created by milagros on 30/10/20.
//

#include "SurpriseBlock.h"

SurpriseBlock::SurpriseBlock() = default;

SurpriseBlock::~SurpriseBlock() = default;

void SurpriseBlock::init(int x, int y, std::string textureID, int currentFrame) {
    Block::init(x, y, textureID, currentFrame);
    direction = 1;
}

void SurpriseBlock::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    _currentFrame = (delayCounter++ % sBlockDelay) ? _currentFrame : ++_currentFrame;
    _currentFrame = (_currentFrame % sBlockFrames);
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, SBWidth, SBHeight,
                                          SBWidth * _currentFrame, renderer, SDL_FLIP_NONE);
    delayCounter++;
}
