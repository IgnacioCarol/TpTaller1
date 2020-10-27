#include "Coin.h"
#include "../GameMap.h"

Coin::Coin() = default;

void Coin::init(int x, int y, std::string textureID, int currentFrame) {
    //int new_x = (int) GameMap::getInstance()->getRandomX(y); //TODO descomentar esto, es para probar lo de las monedas
    GameObject::init(x, y, textureID, currentFrame);
    delayCounter = 0;
}

void Coin::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    _currentFrame = (delayCounter % coinDelay) ? _currentFrame : (_currentFrame + 1) % coinsFrames;
    TextureManager::Instance()->drawFrame(_textureID, xPosition, yPosition, CWidth, CHeight,
                                          CWidth * _currentFrame, renderer, SDL_FLIP_NONE);
    delayCounter++;
}

Coin::~Coin() = default;
