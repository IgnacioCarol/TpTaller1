#include "../Server/GameServer.h"
#include "GameObject.h"
#include "../GameMap.h"

void GameObject::init(int x, int y, std::string textureID) {
    xPosition = x;
    yPosition = y;

    GameMap::getInstance()->insertTo(x,y, this);

    _textureID = textureID;
    _currentFrame = 0;
}

void GameObject::move() {

}

int GameObject::getHeight() {
    return 0;
}

int GameObject::getWidth() {
    return 0;
}

void GameObject::setPosition(int x, int y) {
    xPosition = x;
    yPosition = y;
}

void GameObject::setDirection(bool direction) {

}

void GameObject::setState(std::string state) {

}

int GameObject::getId() {
    return id;
}

GameObjectType GameObject::getType() {
    return type;
}

std::string GameObject::getTextureId() {
    return _textureID;
}

int GameObject::getXPosition() {
    return xPosition;
}

int GameObject::getYPosition() {
    return yPosition;
}

int GameObject::getFrameAmount() {
    return 0;
}

GameObject::GameObject() {
    id = IdGenerator::getInstance()->getNextId();
}

bool GameObject::isAtScene(int camXPos) {
    atScene = (xPosition >= (camXPos - 250) && xPosition <= camXPos + 800) && (yPosition >= 0 && yPosition < 600);
    return atScene;
}

std::string GameObject::getState() {
    return "StaticObject";  //Lo hago para safar ya que las monedas y esas cosas no tienen estado por ahora (quizas nunca tengan)
                            //Licha del pasado casi tuviste razon, pero esto te salvo en el futuro muchas gracias
}

bool GameObject::getDirection() {
    return true; //tmb para safar ya que los bloques y monedas no tienen direction
}

void GameObject::die() {
    GameServer::Instance()->deleteGameObject(this);
}

void GameObject::dieFalling() {

}

void GameObject::collideWith(GameObject *go) {
    die();
}

int GameObject::getFloorPosition() {
    return 0;
}
void GameObject::collideWith(Enemy *enemy) {

}

size_t GameObject::getPoints() {
    return points;
}

void GameObject::fall() {

}
void GameObject::collideWith(Coin *coin) {

}

void GameObject::collideWith(PlatformNormal *nBlock) {

}

int GameObject::centerXPos() {
    return 0;
}

void GameObject::collideWith(Hole *hole) {
}

void GameObject::collideWith(Pipe *pipe) {

}

void GameObject::collideWith(Mushroom *mushroom) {

}

void GameObject::hide() {

}

void GameObject::unhide() {

}

void GameObject::popItem() {

}

void GameObject::setHidden(bool hidden) {
    this->hidden = hidden;
}

bool GameObject::isHidden() {
    return hidden;
}

void GameObject::collideWith(PlatformSurprise* sBlock) {
}

int GameObject::getMinHeightToIntersect() {
    return 60;
}