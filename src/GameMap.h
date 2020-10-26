//
// Created by Daniel Bizari on 26/10/2020.
//

#ifndef TPTALLER1_GAMEMAP_H
#define TPTALLER1_GAMEMAP_H

#include <map>
#include "gameobjects/GameObject.h"

class GameMap {
    public:
        static GameMap * getInstance();
        virtual ~GameMap();
        bool insertTo(size_t x, size_t y, GameObject * actor);
        //Move
        //Remove
        //

    private:
        static GameMap * instance;
        std::map<std::string, GameObject* > gameMap;

        GameMap();
};


#endif //TPTALLER1_GAMEMAP_H
