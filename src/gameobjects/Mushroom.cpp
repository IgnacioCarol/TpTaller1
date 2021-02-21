//
// Created by lisandro on 16/2/21.
//

#include "Mushroom.h"

void Mushroom::init(int xPosition, int yPosition, std::string textureID) {
    GameObject::init(xPosition, yPosition, textureID);
    type = GOT_MUSHROOM;
    //points = POINTS_COIN; chequear si da o no puntos
}

void Mushroom::collideWith(GameObject *go) {
    go->collideWith(this);
}

void Mushroom::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    if (!hidden){
        if (delayCounter == DELAY_MUSHROOM && _currentFrame < MUSHROOM_FRAMES_AMOUNT - 1) {
            _currentFrame += 1;
            delayCounter = 0;
        }
        else delayCounter++;

        TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, MUSHROOM_WIDTH, MUSHROOM_HEIGHT,
                                              MUSHROOM_WIDTH * _currentFrame, renderer, SDL_FLIP_NONE);
    }
}

int Mushroom::getHeight() {
    return MUSHROOM_HEIGHT;
}

int Mushroom::getWidth() {
    return MUSHROOM_WIDTH;
}

void Mushroom::unhide() {
    hidden = false;
    stateType = "UNCATCHED";
}

void Mushroom::hide() {
    hidden = true;
}

std::string Mushroom::getState() {
    return stateType;
}

void Mushroom::setState(std::string newState) {
    if (!hidden && newState != stateType && newState == "UNCATCHED"){
        stateType = newState;
        MusicManager::Instance()->playSound(POWER_UP_APPEARS_SOUND);
    }
    else if (!hidden && newState != stateType && newState == "CATCHED"){
        stateType = newState;
        MusicManager::Instance()->playSound(POWER_UP_SOUND);
    }
}

void Mushroom::changeState(std::string newState) {
    stateType = newState;
}

void Mushroom::move() {
    if (stateType == "CATCHED"){
        GameObject::die();
    }
}