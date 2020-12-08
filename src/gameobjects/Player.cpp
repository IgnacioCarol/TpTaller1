#include <cstdio>
#include <utility>
#include "../CharacterStates/Normal.h"
#include "Player.h"

static const int GRAVITY = 2;

void Player::init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, int framesAmount) {
    GameObject::init(x, y, std::move(textureID));
    xDirection = true;
    jumping = false;
    initialJumpingPosition = yPosition;
    maxYPosition = yPosition - 100;
    cam =  camera;
    characterState = new Normal(0, framesAmount);
}

void Player::run(int direction) {
    if (xPosition < (cam->x + 700) || direction < 0){
        xDirection = direction ? direction > 0 : xDirection;
        xPosition += cam->x < xPosition || direction > 0 ? playerVelocity * direction : 0;
    }
}

void Player::jump(int yMovement) {
    bool isNotStartingPos = yPosition < initialJumpingPosition;
    if ((jumping = canJump() && yMovement)) {
        yPosition = yPosition + (!isNotStartingPos || yMovement ? - (yMovement + GRAVITY - 1) : + GRAVITY); //TODO change velocity to go up
    } else if (isNotStartingPos) {
        yPosition += GRAVITY;
    }
}

bool Player::canJump() const {
    return ((jumping && yPosition > maxYPosition) || (!jumping && yPosition == initialJumpingPosition));
}

Player::Player(SDL_Rect *camera) {
    this->init(0, 380, playerID, camera, 6);
}

void Player::restartPos(int x, int y) {
    xPosition = x;
    yPosition = y;
}

void Player::changeState(CharacterState *newState) {
    delete characterState;
    Logger::getInstance()->debug("Changing Player State");
    characterState = newState;
}

void Player::move() {
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    characterState->changeState(currentKeyStates, this);
    characterState->move(currentKeyStates, this);
}

void Player::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    SDL_RendererFlip flip = (xDirection) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    characterState -> draw(_textureID, xPosition - cameraX, yPosition - cameraY, pWidth, pHeight, renderer, flip);
}

bool Player::isJumping() {
    return jumping;
}

bool Player::finishJump() {
    return initialJumpingPosition == yPosition;
}

Player::~Player() {
    delete characterState;
}
