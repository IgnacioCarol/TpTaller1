#include "PlatformSurprise.h"

void PlatformSurprise::init(int x, int y, std::string textureID) {
    GameObject::init(x, y, textureID);
    type = GOT_PLATFORM_SURPRISE;
}

void PlatformSurprise::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    if(stateType != "EMPTY"){
        _currentFrame = (delayCounter % SURPRISE_BLOCK_DELAY) ? _currentFrame : ((_currentFrame + 1) % (SURPRISE_BLOCK_FRAMES - 1));
    } else {
        _currentFrame = SURPRISE_BLOCK_FRAMES - 1;
    }

    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, SURPRISE_BLOCK_WIDTH, SURPRISE_BLOCK_HEIGHT,
                                          SURPRISE_BLOCK_WIDTH * _currentFrame, renderer, SDL_FLIP_NONE);
    delayCounter++;
}

int PlatformSurprise::getHeight() {
    return SURPRISE_BLOCK_HEIGHT;
}

GameObject *PlatformSurprise::generateItem(int xPos, int yPos) {
    GameObject* tmp;
    std::string itemID;
    if (hasMushroom){
        tmp = new Mushroom();
        itemID = MUSHROOM_ID;
        tmp->init(xPos, yPos - 35, itemID);
    }
    else {
        Coin* coin = new Coin();
        itemID = COIN_ID;
        coin->initInPosition(xPos + 15, yPos - 35, itemID);
        tmp = coin;
    }

    tmp->hide();
    innerItem = tmp;
    return innerItem;
}

void PlatformSurprise::popItem() {
    if (stateType == "FULL") {
        innerItem->unhide();
        stateType = "EMPTY";
    }
}

void PlatformSurprise::setMushroom(bool hasMushroom) {
    this->hasMushroom = hasMushroom;
}

void PlatformSurprise::collideWith(GameObject *go) {
    go->collideWith(this);
}


int PlatformSurprise::getFloorPosition() {
    return - (getHeight() / 4 - 1);
}

std::string PlatformSurprise::getState() {
    return stateType;
}

void PlatformSurprise::setState(std::string newState) {
    if (newState != stateType){
        MusicManager::Instance()->playSound(BUMP_SOUND);
        stateType = newState;
    }
}

int PlatformSurprise::centerXPos() {
    return - SURPRISE_BLOCK_WIDTH / 8;
}

int PlatformSurprise::getWidth() {
    return SURPRISE_BLOCK_WIDTH / 4;
}
