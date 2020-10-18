#include "GameObject.h"

void GameObject::draw() {

    /*TextureManager::Instance()->drawFrame(m_textureID, x, y,
    width, height, currentRow, currentFrame, Stage->renderer);*/ //Comentado porque no tengo el Texture Manager

}

void GameObject::init(size_t x, size_t y, size_t width, size_t height, std::string textureID, size_t frameAmount, currentFrame, currentRow) {
    _x = x;
    _y = y;

    _width = width;
    _height = height;

    _textureID = textureID;

    _currentFrame = currentFrame;
    _currentRow = currentRow;

    _frameAmount = frameAmount; //
}

void GameObject::update() {
   //Movement depends on the GameOject type (different for enemy, player, etc)
}
