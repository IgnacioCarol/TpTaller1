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

void GameObject::setDirection(bool direction) {

}

void GameObject::setState(std::string state) {

}

int GameObject::getId() {
    return id;
}

GameObjectType GameObject::getType() {
    return type;
}

std::string GameObject::getTextureId() {
    return _textureID;
}

int GameObject::getXPosition() {
    return xPosition;
}

int GameObject::getYPosition() {
    return yPosition;
}

int GameObject::getFrameAmount() {
    return 0;
}

GameObject::GameObject() {
    id = IdGenerator::getInstance()->getNextId();
}
