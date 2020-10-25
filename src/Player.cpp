#include <cstdio>
#include <stdio.h>
#include <utility>
#include "Player.h"

void Player::init(size_t x, size_t y, std::string textureID, int currentFrame, SDL_Rect *camera) {
    GameObject::init(x, y, std::move(textureID), currentFrame);
    playerState = "dino"; //dino jumpDino, runDino, asi accedo a la imagen en el map del TextureManager TODO cambiar por clase state
    xDirection = true;
    jumping = false;
    initialJumpingPosition = yPosition;
    maxYPosition = yPosition - 100;
    frames[0]= 0;
    frames[1]= 1;
    frames[2]= 2;
    frames[3]= 3;
    frames[4]= 4;
    cam =  camera;
}

void Player::run(int direction) {
    _currentFrame = (frames[_currentFrame] + 1) % ((sizeof(frames) / sizeof(frames[0])) - 1);
    xDirection = direction ? direction > 0 : xDirection;
    xPosition += cam->x < xPosition || direction > 0 ? direction : 0;
    playerState = (playerState != "jumpDino" && direction) ? "runDino" : playerState;
}

void Player::jump(int yMovement) {
    bool isNotStartingPos = yPosition < initialJumpingPosition;
    if ((jumping = canJump() && yMovement)) {
        yPosition = yPosition + (!isNotStartingPos || yMovement ? - yMovement : + 1);
    } else if (isNotStartingPos) {
        yPosition += 1;
    }
    playerState = (isNotStartingPos) ? "jumpDino" : "dino";
}

bool Player::canJump() const {
    return ((jumping && yPosition > maxYPosition) || (!jumping && yPosition == initialJumpingPosition));
}

Player::Player() {
    this->init(0, 403, std::string(), 0, NULL);
}

void Player::restartPos(int x, int y) {
    xPosition = x;
    yPosition = y;
}
