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
    int objectYPosition = pObject->getYPosition() + pObject->getFloorPosition();
    int realYPosition = first->getYPosition() + first->getFloorPosition();
    int xPosition = first->getXPosition();
    return (abs(xPosition - objectXPosition) < 60) &&
           (abs(realYPosition - objectYPosition) < 30); //ToDo Implement variables
}

void CollisionsManager::checkCollisions(std::vector<GameObject *> goOnScreen, std::vector<GameObject *> players) {
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
            if (player->getState() != "DYING" && go->getState() != "DYING" && this->isInIntersection(player, go)) {
                go->collideWith(player);
            }
        }
    }
}