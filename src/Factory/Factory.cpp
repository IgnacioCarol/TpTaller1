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
                if (tmp != nullptr){
                    textureID = SURPRISE_BLOCK_ID;
                    anchoPlataforma = SBHeight / 4;
                    Logger::getInstance()->debug("Platform surprise created correctly");
                }
                else Logger::getInstance()->error("Error: couldn't create a Surprise Platform");

            } else {
                tmp = new PlatformNormal();
                if (tmp != nullptr){
                    textureID = NORMAL_BLOCK_ID;
                    anchoPlataforma = NBHeight / 4;
                    Logger::getInstance()->debug("Normal platform created correctly");
                }
                else Logger::getInstance()->error("Error: couldn't create a Normal Platform");
            }

            if (tmp != nullptr){
                TextureManager::Instance() -> addPath(textureID, platform.image, DEFAULT_PLATFORM_PATH);
                tmp->init(platform.coordX + i * anchoPlataforma, platform.coordY, textureID, 0);

                actors.push_back(tmp);
            }
        }
    }

    // Init Coins
    for(auto coin : levelConfig.coins) {
        for(size_t i = 0; i < coin.quantity; i++) {
            tmp = new Coin();
            if (tmp != nullptr){ //TODO we can initialice the Y position randomly too
                TextureManager::Instance()->addPath(COIN_ID, coin.image, DEFAULT_COIN_PATH);
                tmp->init(0, coin.coordY, COIN_ID, 0);

                actors.push_back(tmp);
                Logger::getInstance()->debug("Coin created correctly");
            }
            else Logger::getInstance()->error("Error: couldn't create a Coin");
        }
    }

    for(auto enemies : levelConfig.enemies) {
        for(size_t i = 0; i < enemies.quantity; i++) {
            if (enemies.type == ENEMY_TURTLE) { //TodO we need more types for the different enemies like koopaGreen, koopaRed, etc
                tmpEnemy = new EnemyTurtle();
                if (tmpEnemy != nullptr){
                    TextureManager::Instance() -> addPath(KOOPA_GREEN_ID, enemies.image, DEFAULT_TURTLE_PATH);
                    tmpEnemy->init(900, 435, KOOPA_GREEN_ID, 0, Game::Instance()->getCamera(), 3,
                                   new EnemyMovement(0, 3));
                    Logger::getInstance()->debug("Turtle enemy created correctly");
                }
                else Logger::getInstance()->error("Error: couldn't create a Turtle Enemy");

            } else {
                tmpEnemy = new EnemyMushroom();
                if (tmpEnemy != nullptr){
                    TextureManager::Instance() -> addPath(GOOMBA_ID, enemies.image, DEFAULT_MUSHROOM_PATH);
                    tmpEnemy->init(900, 425, GOOMBA_ID, 0, Game::Instance()->getCamera(), 5, new EnemyMovement(0, 5));
                    Logger::getInstance()->debug("Mushroom enemy created correctly");
                }
                else Logger::getInstance()->error("Error: couldn't create a Mushroom Enemy");
            }
            if (tmpEnemy != nullptr){
                tmp = tmpEnemy;
                actors.push_back(tmp);
            }
        }
    }


    return actors;
}

Factory::~Factory() = default;

