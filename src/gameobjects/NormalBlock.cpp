#include "NormalBlock.h"

NormalBlock::NormalBlock() = default;

NormalBlock::~NormalBlock() = default;

void NormalBlock::init(int x, int y, std::string textureID, int currentFrame) {
    Block::init(x, y, textureID, currentFrame);
}

void NormalBlock::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, NBWidth, NBHeight,
                                          0, renderer, SDL_FLIP_NONE);
}
