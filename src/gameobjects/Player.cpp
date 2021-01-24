#include <cstdio>
#include <utility>
#include <src/Server/GameServer.h>
#include <src/Utils/CollisionsManager.h>
#include "../CharacterStates/Paused.h"
#include "../CharacterStates/Normal.h"
#include "../CharacterStates/Jumping.h"
#include "../CharacterStates/Crouched.h"
#include "../CharacterStates/Running.h"

#include "Player.h"

static const int GRAVITY = 3;

void Player::init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, int framesAmount) {
    GameObject::init(x, y, std::move(textureID));
    xDirection = true;
    jumping = false;
    initialJumpingPosition = yPosition;
    maxYPosition = yPosition - 100;
    cam =  camera;
    characterState = new Normal(0, framesAmount);
    type = GOT_PLAYER;
    ticks = 0;
    leftOrRightPressed = false;
    atScene = true; //Maybe we never gonna ask this to the player, but never knows...
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

Player::Player(SDL_Rect *camera, std::string username, std::string textureID) : GameObject() {
    this->init(0, 380, textureID, camera, 6);
    this->username = username;
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

void Player::move(std::vector<int> vector) {
    Uint8 keyStates[83];
    ticks = 0;
    std::vector<int> arrows = {SDL_SCANCODE_UP, SDL_SCANCODE_LEFT, SDL_SCANCODE_DOWN, SDL_SCANCODE_RIGHT};
    for (int i = 0; i < vector.size() && i < arrows.size(); i++) {
        keyStates[arrows[i]] = vector[i];
    }
    leftOrRightPressed = vector[1] || vector[3];
    completeMovement(keyStates);
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

std::string Player::getUsername() {
    return username;
}

void Player::setUsername(std::string username) {
    this->username = username;
}

int Player::getFrameAmount() {
    return characterState->getFramesAmount();
}

void Player::setPosition(int x, int y) {
    xPosition = x;
    yPosition = y;
}

void Player::setDirection(bool direction) {
    this -> xDirection = direction;
}

void Player::setState(std::string state) {
    if (state != characterState->getStateType()) {
        int framesAmount = characterState->getFramesAmount();
        if (state == "JUMPING") {
            changeState(new Jumping(4, framesAmount));
        } else if (state == "NORMAL") {
            changeState(new Normal(0, framesAmount));
        } else if (state == "RUNNING") {
            changeState(new Running(0, framesAmount));
        } else if (state == "CROUCHED") {
            changeState(new Crouched(5, framesAmount));
        }
        else if (state == "PAUSED" || state == "FINISH"){
            changeState(new Paused(0, framesAmount, state == "PAUSED"));
        }
    }
}
std::string Player::getState() {
    return characterState->getStateType();
}

bool Player::getDirection() {
    return xDirection;
}

void Player::move() {
    Uint8 keyStates[83];
    if (ticks < 50) {
        keyStates[SDL_SCANCODE_UP] = isJumping();
        bool isMoving = leftOrRightPressed && (characterState->getStateType() == "JUMPING" || characterState->getStateType() == "RUNNING");
        keyStates[SDL_SCANCODE_LEFT] =  isMoving && !xDirection;
        keyStates[SDL_SCANCODE_RIGHT] = isMoving && xDirection;
        keyStates[SDL_SCANCODE_DOWN] = characterState->getStateType() == "CROUCHED";
        ticks++;
    } else {
        keyStates[SDL_SCANCODE_UP] = keyStates[SDL_SCANCODE_LEFT] = keyStates[SDL_SCANCODE_RIGHT] = keyStates[SDL_SCANCODE_DOWN] = 0;
    }
    completeMovement(keyStates);
}

void Player::completeMovement(const Uint8 *keyStates) {
    auto imageMap = TextureManager::Instance()->getTextureMap();
    auto gameServer = GameServer::Instance();
    for (auto gameObject: gameServer->getGameObjectsOnScreen()) {
        if (CollisionsManager::Instance()->isInIntersection(this, gameObject)) {
            gameObject->collideWith(this);
        }
    }
    characterState->changeState(keyStates, this);
    characterState->move(keyStates, this);
}

void Player::collideWith(GameObject *go) {

}

void Player::die() {
    restartPos(GameServer::Instance()->getCamera()->getXpos(), 380); //This is current die of the player, we should implement
    //lives and all of that, but this could stay for test enviroment
}