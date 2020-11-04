#include "GameObject.h"
#include "../GameMap.h"

void GameObject::init(int x, int y, std::string fileName, std::string defaultImg, std::string textureID, int currentFrame) {
    xPosition = x;
    yPosition = y;

    GameMap::getInstance()->insertTo(x,y, this);

    _textureID = textureID;
    _fileName = fileName;
    _defaultImg = defaultImg;

    _currentFrame = currentFrame;
}

void GameObject::move() {

}

std::string GameObject::getFilePath() {
    return _fileName;
}

std::string GameObject::getID() {
    return _textureID;
}

std::string GameObject::getDefault() {
    return _defaultImg;
}
