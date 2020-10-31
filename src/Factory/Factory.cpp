#include "Factory.h"
#include <vector>
#include "../gameobjects/Coin.h"
#include "../gameobjects/PlatformNormal.h"
#include "../gameobjects/PlatformSurprise.h"

Factory* Factory::instance = nullptr;

Factory * Factory::getInstance() {
    if (Factory::instance == nullptr) {
        Factory::instance = new Factory();
    }

    return instance;
}

Factory::Factory() = default;

std::vector<GameObject*> Factory::createGameObjectsFromLevelConfig(Level levelConfig) {
    std::vector<GameObject*> actors;
    GameObject * tmp;
    int anchoPlataforma = 2; //TODO determinar que valor es el correcto, quiza convenga que sea configurable desde el XML
    std::string textureID;

    // Init Platforms
    for(auto platform : levelConfig.platforms) {
        for(size_t i = 0; i < platform.quantity; i++) {
            if (platform.type == PLATFORM_SURPRISE) {
                tmp = new PlatformSurprise();
                textureID = nBlockID;
            } else {
                tmp = new PlatformNormal();
                textureID = sBlockID;
            }

            tmp->init(platform.coordX + i * anchoPlataforma, platform.coordY,
                      textureID, 0);

            actors.push_back(tmp);
        }
    }

    // Init Coins
    for(auto coin : levelConfig.coins) {
        for(size_t i = 0; i < coin.quantity; i++) {
            tmp = new Coin();
            tmp->init(0, coin.coordY, //Position x determined by init randomly
                      coinsID, 0);

            actors.push_back(tmp);
        }
    }
/*
    for(auto enemies : levelConfig.enemies) {
        for(size_t i = 0; i < enemies.quantity; i++) {
            if (enemies.type == ENEMY_TURTLE) {
                tmp = new xmlEnemy();
            } else {
                tmp = new EnemyMushroom();
            }

            tmp->init(0, 0, //Position x and y determined by init randomly
                      "sarasa", 0); //TODO definir lo de texture y current frame

            actors.push_back(tmp);
        }
    }
*/

    return actors;
}

Factory::~Factory() = default;

