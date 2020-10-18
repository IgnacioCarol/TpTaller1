#include "Player.h"

void Player::init(size_t x, size_t y, size_t width, size_t height, std::string textureID, size_t frameAmount, currentFrame, currentRow) {
    GameObject::init(x, y, width, height, textureID, frameAmount, currentFrame, currentRow);
}

void Player::draw() {
    GameObject::draw();
}

void Player::update() {
    GameObject::update();
   /* m_currentFrame = int(((SDL_GetTicks() / 100) % m_frameAmount)); //To do: ver update desde teclado
}