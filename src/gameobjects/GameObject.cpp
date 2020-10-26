#include "GameObject.h"

void GameObject::init(int x, int y, std::string textureID, int currentFrame) {
    xPosition = x;
    yPosition = y;

    _textureID = textureID;

    _currentFrame = currentFrame;
}
