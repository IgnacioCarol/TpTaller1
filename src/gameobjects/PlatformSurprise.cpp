#include "PlatformSurprise.h"

void PlatformSurprise::init(int x, int y, std::string textureID) {
    GameObject::init(x, y, textureID);
    type = GOT_PLATFORM_SURPRISE;
}

void PlatformSurprise::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    if(containsItem){
        _currentFrame = (delayCounter % SURPRISE_BLOCK_DELAY) ? _currentFrame : ((_currentFrame + 1) % (SURPRISE_BLOCK_FRAMES - 1));
    } else {
        _currentFrame = SURPRISE_BLOCK_FRAMES;
    }

    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, SURPRISE_BLOCK_WIDTH, SURPRISE_BLOCK_HEIGHT,
                                          SURPRISE_BLOCK_WIDTH * _currentFrame, renderer, SDL_FLIP_NONE);
    delayCounter++;
}

int PlatformSurprise::getHeight() {
    return SURPRISE_BLOCK_HEIGHT;
}

GameObject *PlatformSurprise::generateItem() {
    GameObject* tmp;
    std::string itemID;
    if (hasMushroom){
        tmp = new Mushroom();
        itemID = MUSHROOM_ID;
    }
    else {
        tmp = new Coin();
        itemID = COIN_ID;
    }
    int xPos = (hasMushroom) ? 300 : 400;
    tmp->init(xPos, xPos, itemID); //ToDo cuando funcione chquear las posiciones a pasar aca
    tmp->hide();
    innerItem = tmp;
    return innerItem;
}

void PlatformSurprise::popItem() {
    if (containsItem) {
        containsItem = false;
        innerItem->unhide();
    }
}
void PlatformSurprise::setMushroom(bool hasMushroom) {
    this->hasMushroom = hasMushroom;
}

bool PlatformSurprise::containsMushroom() {
    return this->hasMushroom;
}

void PlatformSurprise::collideWith(GameObject *go) {
    go->collideWith(this);
}

/*void PlatformSurprise::collideWith(Player *player) {
    std::pair<int, int> position = player->getPosition();
    int xPlayer = position.first;
    int yPlayer = position.second;
    if(xPlayer > xPosition && xPlayer < (xPosition + SURPRISE_BLOCK_WIDTH)){
        if(yPlayer >= (yPosition + SURPRISE_BLOCK_HEIGHT)){
            player->setPosition(xPlayer, yPosition + SURPRISE_BLOCK_HEIGHT);
        }
        if(yPlayer == yPosition && containsItem){ //TODO refactor esto segun desde donde midamos el y (pies o cabeza)
            //TODO implementar hongo
            containsItem = false;
        }
    }
}
*/