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
        size_t getRandomX(size_t y);
        //Move
        //Remove
        //

    private:
        static GameMap * instance;
        std::map<std::string, GameObject* > gameMap;

        GameMap();
        std::string buildKey(size_t x, size_t y);
};


#endif //TPTALLER1_GAMEMAP_H
