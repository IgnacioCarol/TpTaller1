//
// Created by nacho on 24/1/21.
//

#ifndef TPTALLER1_COLLISIONSMANAGER_H
#define TPTALLER1_COLLISIONSMANAGER_H
#include <src/gameobjects/GameObject.h>
#include <src/Server/GameServer.h>

class CollisionsManager {
public:
    bool isInIntersection(GameObject *first, GameObject *pObject);

    static CollisionsManager *Instance();
    void checkCollisions();

private:
    static CollisionsManager* instance;
};


#endif //TPTALLER1_COLLISIONSMANAGER_H
