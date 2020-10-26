//
// Created by Daniel Bizari on 25/10/2020.
//

#include "Factory.h"
#include <vector>

Factory* Factory::instance = nullptr;

Factory * Factory::getInstance() {
    if (Factory::instance == nullptr) {
        Factory::instance = new Factory();
    }

    return instance;
}

Factory::Factory() = default;

std::vector<GameObject> Factory::createGameObjectsFromLevelConfig(Level levelConfig) {
    std::vector<GameObject> actors;

    // Init Coins
    for(auto coin : levelConfig.coins) {
//        actors.push_back(new Coin());
    }

    // Init Blocks
    for(auto platform : levelConfig.platforms) {
        //ToDo determinar tipo de plataforma e instanciar
//        actors.push_back(new Coin());
    }

    for(auto platform : levelConfig.enemies) {
        //ToDo determianr que tipo de enemigo instanciar segun numero de enemigo
//                actors.push_back(new Coin());
    }


    // Init
    return std::vector<GameObject>();
}

Factory::~Factory() = default;

