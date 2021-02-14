#include "Coin.h"
#include "../GameMap.h"

void Coin::init(int x, int y, std::string textureID) {
    int new_x = (int) GameMap::getInstance()->getRandomX(y);
    GameObject::init(new_x, y, textureID);
    _currentFrame = 0;
    delayCounter = 0;
    type = GOT_COIN;
    points = POINTS_COIN;
}

void Coin::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    _currentFrame = (delayCounter % COIN_DELAY) ? _currentFrame : (_currentFrame + 1) % COIN_FRAMES;
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, COIN_WIDTH, COIN_HEIGHT,
                                          COIN_WIDTH * _currentFrame, renderer, SDL_FLIP_NONE);
    delayCounter++;
}

void Coin::collideWith(GameObject *go) {
    go->collideWith(this);
}

Coin::~Coin() = default;
