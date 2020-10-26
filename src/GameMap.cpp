//
// Created by Daniel Bizari on 26/10/2020.
//

#include "GameMap.h"
#include <sstream>

GameMap* GameMap::instance = nullptr;

GameMap * GameMap::getInstance() {
    if (GameMap::instance == nullptr) {
        GameMap::instance = new GameMap();
    }

    return instance;
}

bool GameMap::insertTo(size_t x, size_t y, GameObject * actor) {
    std::stringstream stream;
    std::string key;

    stream << x << "-" << y;
    key = stream.str();

    if (this->gameMap.find(key) != this->gameMap.end()) {
        return false;
    }

    this->gameMap[key] = actor;
    return true;
}



GameMap::GameMap() = default;

GameMap::~GameMap() = default;


