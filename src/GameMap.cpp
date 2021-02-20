//
// Created by Daniel Bizari on 26/10/2020.
//

#include "GameMap.h"
#include <sstream>
#include <random>
#include <iostream>

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
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(300.0, 3420.0);

    for (int i=0; i<16; ++i)
        std::cout << dist(mt) << "\n";

    while (!found) {
//        result = 300 + (dist(mt) % 3440 + 1); //TODO averiguar rango de x del mapa, quiza esto depende del mapa asi que habria que ver si es necesario pasarle por parametro rango max
        result = dist(mt);
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


GameMap::GameMap() = default;

GameMap::~GameMap() = default;


