//
// Created by nacho on 24/1/21.
//

#ifndef TPTALLER1_COLLISIONSMANAGER_H
#define TPTALLER1_COLLISIONSMANAGER_H


class CollisionsManager {
public:
    bool isInIntersection(GameObject *first, GameObject *pObject);

    void doIntersection();

    static CollisionsManager *Instance();

private:
    CollisionsManager* instance;
};


#endif //TPTALLER1_COLLISIONSMANAGER_H
