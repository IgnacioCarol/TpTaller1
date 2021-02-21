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

bool CollisionsManager::isInIntersection(GameObject *go1, GameObject *go2) {
    int yPosition1 = go1->getYPosition() + go1->getFloorPosition();
    int xPosition1 = go1->getXPosition() + go1->centerXPos();
    int xPosition2 = go2->getXPosition() + go2->centerXPos();
    int yPosition2 = go2->getYPosition() + go2->getFloorPosition();

    if (go1->getType() == GOT_PLAYER && go2->getType() == GOT_HOLE && go1->getState() == "JUMPING") {
        yPosition1 -= 10;
    }

    return (abs(xPosition1 - xPosition2) <= std::max(go1->getWidth(), go2->getWidth()) / calculateCollisionDivFactor(go1,go2)) &&
           (abs(yPosition1 - yPosition2) <= std::max(go1->getMinHeightToIntersect(), go2->getMinHeightToIntersect()));
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
            if (player->getState() != "DYING"  && player->getState() != "PAUSED" && go->getState() != "DYING" && this->isInIntersection(player, go)) {
                go->collideWith(player);
            }
        }
    }
}

float CollisionsManager::calculateCollisionDivFactor(GameObject *go1, GameObject *go2) {
    if ((go1->getType() == GOT_PLAYER && go2->getType() == GOT_PLATFORM_NORMAL) ||
        (go2->getType() == GOT_PLAYER && go1->getType() == GOT_PLATFORM_NORMAL) ||
        (go1->getType() == GOT_PLAYER && go2->getType() == GOT_PLATFORM_SURPRISE) ||
        (go2->getType() == GOT_PLAYER && go1->getType() == GOT_PLATFORM_SURPRISE)) {
        return 2.8;
    }

    if ((go1->getType() == GOT_PLAYER && go2->getType() == GOT_COIN) ||
        (go2->getType() == GOT_PLAYER && go1->getType() == GOT_COIN)) {
        return 3.8;
    }

    return 2.0;
}