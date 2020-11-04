#include "Factory.h"
#include <vector>
#include "../gameobjects/Coin.h"
#include "../gameobjects/PlatformNormal.h"
#include "../gameobjects/PlatformSurprise.h"
#include "../gameobjects/EnemyMushroom.h"
#include "../gameobjects/EnemyTurtle.h"
#include "../CharacterStates/EnemyMovement.h"
#include "../Game.h"

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
    Enemy* tmpEnemy;
    int anchoPlataforma;
    std::string textureID;

    // Init Platforms
    for(auto platform : levelConfig.platforms) {
        for(size_t i = 0; i < platform.quantity; i++) {
            if (platform.type == PLATFORM_SURPRISE) {
                tmp = new PlatformSurprise();
                textureID = sBlockID;
                anchoPlataforma = SBHeight / 4;
            } else {
                tmp = new PlatformNormal();
                textureID = nBlockID;
                anchoPlataforma = NBHeight / 4;
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

    for(auto enemies : levelConfig.enemies) {
        for(size_t i = 0; i < enemies.quantity; i++) {
            if (enemies.type == ENEMY_TURTLE) {
                tmpEnemy = new EnemyTurtle();
                tmpEnemy -> init(900, 415, etID, 0, Game::Instance() -> getCamera() , 3, new EnemyMovement(0, 3));

            } else {
                tmpEnemy = new EnemyMushroom();
                tmpEnemy -> init(900, 425, emID, 0, Game::Instance() -> getCamera() , 5, new EnemyMovement(0, 5));

            }
            tmp = tmpEnemy;

            actors.push_back(tmp);
        }
    }


    return actors;
}

Factory::~Factory() = default;

