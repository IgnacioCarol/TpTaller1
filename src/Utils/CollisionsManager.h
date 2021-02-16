//
// Created by nacho on 24/1/21.
//

#ifndef TPTALLER1_COLLISIONSMANAGER_H
#define TPTALLER1_COLLISIONSMANAGER_H
#include "../gameobjects/GameObject.h"

class CollisionsManager {
public:
    bool isInIntersection(GameObject *go1, GameObject *go2);

    static CollisionsManager *Instance();
    void checkCollisions(std::vector<GameObject *> goOnScreen, std::vector<GameObject *> players);

private:
    static CollisionsManager* instance;
};


#endif //TPTALLER1_COLLISIONSMANAGER_H
