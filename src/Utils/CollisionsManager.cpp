//
// Created by nacho on 24/1/21.
//

#include <src/gameobjects/GameObject.h>
#include <src/Server/GameServer.h>
#include "CollisionsManager.h"

bool CollisionsManager::isInIntersection(GameObject *first, GameObject *pObject) {
    int objectXPosition = pObject->getXPosition();
    int objectYPosition = pObject->getYPosition();
    int realYPosition = first->getYPosition() + 45; //Fixme this is to make a semi starting floor, should be fixed
    int xPosition = first->getXPosition();
    return (xPosition >= objectXPosition && xPosition <= objectXPosition + 30) &&
           (realYPosition <= objectYPosition && realYPosition >= objectYPosition - 30); //ToDo Implement variables
}

void CollisionsManager::doIntersection() {
    auto coso = GameServer::Instance()->getGameObjectsOnScreen();
    for (auto gameObject: coso) {
/*        if (gameObject->canInitiateCollitions()) {
            gameObject->collideWith(otherObject);
        }*/
    }
}