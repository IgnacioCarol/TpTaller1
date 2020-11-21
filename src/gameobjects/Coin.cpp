#include "Coin.h"
#include "../GameMap.h"

Coin::Coin() = default;

void Coin::init(int x, int y, std::string textureID) {
    int new_x = (int) GameMap::getInstance()->getRandomX(y);
    GameObject::init(new_x, y, textureID);
    _currentFrame = 0;
    delayCounter = 0;
}

void Coin::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    _currentFrame = (delayCounter % COIN_DELAY) ? _currentFrame : (_currentFrame + 1) % COIN_FRAMES;
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, COIN_WIDTH, COIN_HEIGHT,
                                          COIN_WIDTH * _currentFrame, renderer, SDL_FLIP_NONE);
    delayCounter++;
}

Coin::~Coin() = default;
