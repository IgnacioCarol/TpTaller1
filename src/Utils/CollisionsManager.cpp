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

bool CollisionsManager::isInIntersection(GameObject *go, GameObject *anotherGo) {
    int objectXPosition = anotherGo->getXPosition() + anotherGo->centerXPos();
    int objectYPosition = anotherGo->getYPosition() + anotherGo->getFloorPosition();
    int realYPosition = go->getYPosition() + go->getFloorPosition();
    int xPosition = go->getXPosition() + go->centerXPos();
    return (abs(xPosition - objectXPosition) < std::max(go->getWidth(), anotherGo->getWidth()) / 2) &&
           (abs(realYPosition - objectYPosition) < 60); //ToDo Implement variables
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