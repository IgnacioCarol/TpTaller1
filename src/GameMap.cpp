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

    key = buildKey(x, y);

    if (this->gameMap.find(key) != this->gameMap.end()) {
        return false;
    }

    this->gameMap[key] = actor;
    return true;
}

size_t GameMap::getRandomX(size_t y) {
    std::string key;
    size_t result = 0;
    bool found = false;

    while (!found) {
        result = rand() % 3840 + 1; //TODO averiguar rango de x del mapa, quiza esto depende del mapa asi que habria que ver si es necesario pasarle por parametro rango max
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


