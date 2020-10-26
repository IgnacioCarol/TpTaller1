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

    // Init Blocks
    for(auto platform : levelConfig.platforms) {
        if (platform.type == PLATFORM_NORMAL) {
            // instanciar plataforma normal
        } else if (platform.type == PLATFORM_SURPRISE) {
            // instanciar plataforma sorpresa
        }
//      actors.push_back(new Coin()); TODO crear clases
    }

    // Init Coins
    for(auto coin : levelConfig.coins) {
//        actors.push_back(new Coin());
    }

    for(auto enemies : levelConfig.enemies) {
        //ToDo determianr que tipo de enemigo instanciar segun numero de enemigo
//                actors.push_back(new Coin());
    }
//TODO Crear clase mapa, que sea un hashmap y que le pueda pedir dame un random x,

    // Init
    return std::vector<GameObject>();
}

Factory::~Factory() = default;

