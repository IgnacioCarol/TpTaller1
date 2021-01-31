//
// Created by nacho on 24/1/21.
//


#include "CollisionsManager.h"

CollisionsManager* CollisionsManager::instance = nullptr;

CollisionsManager *CollisionsManager::Instance() {
    //First time we create an instance of Collision manager
    if (instance == nullptr) instance = new CollisionsManager();
    return instance;
}

bool CollisionsManager::isInIntersection(GameObject *first, GameObject *pObject) {
    int objectXPosition = pObject->getXPosition();
    int objectYPosition = pObject->getYPosition();
    int realYPosition = first->getYPosition() + 45; //Fixme this is to make a semi starting floor, should be fixed
    int xPosition = first->getXPosition();
    return (xPosition >= objectXPosition && xPosition <= objectXPosition + 30) &&
           (realYPosition <= objectYPosition && realYPosition >= objectYPosition - 30); //ToDo Implement variables
}

void CollisionsManager::checkCollisions() {
    GameServer* gs = GameServer::Instance();
    auto goOnScreen = gs->getGameObjectsOnScreen();
    auto players = gs->getPlayers();
    for (auto go : goOnScreen) {
        auto type = go->getType();
        if (type == GOT_COIN || type == GOT_ENEMY_MUSHROOM || type == GOT_ENEMY_TURTLE) {
            continue;
        }
        for (auto secondGo : goOnScreen) {
            if (go == secondGo) {
                continue;
            }
            if (this->isInIntersection(go, secondGo)) {
                go->collideWith(secondGo);
            }
        }
    }
    for (auto player : players) {
        for (auto go : goOnScreen) {
            if (this->isInIntersection(player, go)) {
                go->collideWith(player);
            }
        }
    }
}