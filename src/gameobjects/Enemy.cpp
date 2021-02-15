#include "Enemy.h"


void Enemy::init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, CharacterState *state) {
    GameObject::init(x, y, std::move(textureID));
    enemyState = state;
    cam = camera;
}

void Enemy::move() {
    enemyState -> move(this);
}

void Enemy::walk() {
    xPosition += direction;
    if (yPosition < floor) {
        yPosition++;
    }
    flipFlag = direction == enemyVelocity;
}

void Enemy::draw(SDL_Renderer *renderer, int cameraX, int cameraY, size_t width, size_t height) {
    SDL_RendererFlip flip = (!flipFlag) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    enemyState -> draw(_textureID, xPosition - cameraX, yPosition - cameraY, width, height, renderer, flip);
}

Enemy::~Enemy() {
    delete this->enemyState;
}

int Enemy::getFrameAmount() {
    return enemyState->getFramesAmount();
}

void Enemy::setPosition(int x, int y) {
    xPosition = x;
    yPosition = y;
}

void Enemy::setDirection(bool direction) {
    this -> flipFlag = direction; //false: der a izq true: izq a der
}

void Enemy::setState(std::string state) {
    if (state != enemyState->getStateType() && state == "DYING"){
        this->die();
    }
}

std::string Enemy::getState() {
    return enemyState->getStateType();
}

bool Enemy::getDirection() {
    return direction > 0;
}

void Enemy::collideWith(GameObject *go) {
   go->collideWith(this);
}

void Enemy::standOrRevertMovement(GameObject *go, int heigth) {
    int yBlock = go->getYPosition() + go->getFloorPosition();
    if(yPosition + getFloorPosition() + 20 < yBlock) {
        yPosition = yBlock - heigth - getFloorPosition();
    } else {
        direction = -1 * direction;
    }
}

void Enemy::collideWith(Pipe *pipe) {
    standOrRevertMovement(pipe, pipe->getHeight() / 4 - 95);
}

void Enemy::collideWith(PlatformNormal *nBlock) {
    standOrRevertMovement(nBlock, nBlock->getHeight() - 50);
}

size_t Enemy::getPoints() {
    return pointsToPlayer;
}


void Enemy::die() {
    if (enemyState->getStateType() == "DYING"){
        GameObject::die();
    }
    int framesAmount = enemyState->getFramesAmount();
    delete enemyState;
    enemyState = new Dying(framesAmount - 1);
}

void Enemy::dieFalling() {
    GameObject::die();
}
