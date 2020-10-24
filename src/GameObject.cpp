#include "GameObject.h"

void GameObject::init(int x, int y, size_t width, size_t height, std::string textureID, int currentFrame) {
    xPosition = x;
    yPosition = y;

    _width = width;
    _height = height;

    _textureID = textureID;

    _currentFrame = currentFrame;
}
