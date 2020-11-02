#include "GameObject.h"
#include "../GameMap.h"

void GameObject::init(int x, int y, std::string textureID, int currentFrame) {
    xPosition = x;
    yPosition = y;

    GameMap::getInstance()->insertTo(x,y, this);

    _textureID = textureID;

    _currentFrame = currentFrame;
}

void GameObject::move() {

}
