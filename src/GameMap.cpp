//
// Created by Daniel Bizari on 26/10/2020.
//

#include "GameMap.h"
#include <sstream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

GameMap* GameMap::instance = nullptr;

GameMap * GameMap::getInstance() {
    if (GameMap::instance == nullptr) {
        GameMap::instance = new GameMap();
    }

    return instance;
}

bool GameMap::insertTo(size_t x, size_t y, GameObject * actor) {
    std::string key;

    for(int height = 0; height < actor->getHeight(); height++) {
        for(int width = 0; width < actor->getWidth(); width++) {
            key = buildKey(x + height, y + width);

            if (this->gameMap.find(key) != this->gameMap.end()) { // ToDo agregar reglas, ej solo devolver false en caso de enemigos con pipes
                GameObject * go = this->gameMap[key];
                if ((actor->getType() == GOT_ENEMY_MUSHROOM || actor->getType() == GOT_ENEMY_TURTLE) && go->getType() == GOT_PIPE) {
                    std::stringstream ss;
                    ss << "[event:InsertGameObjectToMap][error:squareAlreadyOccupied] x: " << x + height << " y: " <<  y + width <<" occupied by id: " << go->getId() << " type: " << go->getType();
                    std::string sst = ss.str();
                    Logger::getInstance()->debug(ss.str());
                    return false;
                }
            }
        }
    }

    for(int height = 0; height < actor->getHeight(); height++) {
        for(int width = 0; width < actor->getWidth(); width++) {
            key = buildKey(x + height, y + width);
            this->gameMap[key] = actor;
        }
    }

    return true;
}

size_t GameMap::getRandomX(size_t y) {
    std::string key;
    size_t result = 0;
    bool found = false;

    while (!found) {
        result = 300 + (rand() % 3440 + 1); //TODO averiguar rango de x del mapa, quiza esto depende del mapa asi que habria que ver si es necesario pasarle por parametro rango max
        key = buildKey(result, y);
        if (this->gameMap.find(key) == this->gameMap.end()) {
            found = true;
        }
    }

    return result;
}

std::string GameMap::buildKey(size_t x, size_t y) {
    std::stringstream stream;

    stream << x << "-" << y;
    return stream.str();
}

size_t GameMap::getRandomX() {
    return rand() % MAP_WIDTH;
}


GameMap::GameMap() = default;

GameMap::~GameMap() = default;


