#include "GameObject.h"
#include "../GameMap.h"

void GameObject::init(int x, int y, std::string textureID) {
    xPosition = x;
    yPosition = y;

    GameMap::getInstance()->insertTo(x,y, this);

    _textureID = textureID;
    _currentFrame = 0;
}

void GameObject::move() {

}

int GameObject::getHeight() {
    return 0;
}

void GameObject::setPosition(int x, int y) {

}

void GameObject::setDirection(int direction) {

}

void GameObject::setState(std::string state) {

}
