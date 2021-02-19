#include "Coin.h"
#include "../GameMap.h"

void Coin::init(int x, int y, std::string textureID) {
    int new_x = (int) GameMap::getInstance()->getRandomX(y);
    initInPosition(new_x, y, textureID);
}

void Coin::initInPosition(int x, int y, std::string textureID) {
    GameObject::init(x, y, textureID);
    _currentFrame = 0;
    delayCounter = 0;
    type = GOT_COIN;
    points = POINTS_COIN;
}

void Coin::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    if (!hidden){
        _currentFrame = (delayCounter % COIN_DELAY) ? _currentFrame : (_currentFrame + 1) % COIN_FRAMES;
        TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, COIN_WIDTH, COIN_HEIGHT,
                                              COIN_WIDTH * _currentFrame, renderer, SDL_FLIP_NONE);
        delayCounter++;
    }
}

void Coin::move() {
    if (stateType == "CATCHED"){
        GameObject::die();
    }
}

void Coin::collideWith(GameObject *go) {
    go->collideWith(this);
}

void Coin::hide() {
    hidden = true;
}

void Coin::unhide() {
    hidden = false;
}

int Coin::getFloorPosition() {
    return - (COIN_HEIGHT / 4  + 30);
}

int Coin::getHeight() {
    return COIN_HEIGHT;
}

int Coin::getWidth() {
    return COIN_WIDTH;
}

std::string Coin::getState() {
    return stateType;
}

void Coin::setState(std::string newState) {
    if (stateType != newState){
        MusicManager::Instance()->playSound(COIN_SOUND);
        stateType = newState;
    }
}

void Coin::changeState(std::string newState) {
    stateType = newState;
}

Coin::~Coin() = default;
