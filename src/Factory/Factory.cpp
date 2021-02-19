#include "Factory.h"
#include <vector>
#include "../gameobjects/Coin.h"
#include "../gameobjects/Pipe.h"
#include "../gameobjects/PlatformNormal.h"
#include "../gameobjects/PlatformSurprise.h"
#include "../gameobjects/EnemyMushroom.h"
#include "../gameobjects/EnemyTurtle.h"
#include "../CharacterStates/EnemyMovement.h"
#include "../Server/GameServer.h"
#include "../gameobjects/Hole.h"

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
    std::vector<GameObject*> goPlatforms;
    std::vector<GameObject*> goCoins;
    std::vector<GameObject*> goEnemies;
    std::vector<GameObject*> goPipes;

    GameObject * tmp;
    Enemy* tmpEnemy;
    int platformHeight;
    std::string textureID;


    // Init Platforms
    for(auto platform : levelConfig.platforms) {
        int mushroomCreated = 0;
        for(long i = 0; i < platform.quantity; i++) {
            if (platform.type == PLATFORM_SURPRISE) {
                PlatformSurprise * ps = new PlatformSurprise();
                tmp = ps;
                if (tmp != nullptr){
                    textureID = SURPRISE_BLOCK_ID;
                    ps->setMushroom(mushroomCreated < platform.mushroomQuantity);
                    mushroomCreated++;
                    actors.push_back(ps->generateItem(platform.coordX + i * platformHeight, platform.coordY));
                    platformHeight = tmp -> getHeight() / 4;
                    Logger::getInstance()->debug("Platform surprise created correctly");
                }
                else Logger::getInstance()->error("Error: couldn't create a Surprise Platform");

            } else {
                tmp = new PlatformNormal();
                if (tmp != nullptr){
                    textureID = NORMAL_BLOCK_ID;
                    platformHeight = tmp->getHeight();
                    Logger::getInstance()->debug("Normal platform created correctly");
                }
                else Logger::getInstance()->error("Error: couldn't create a Normal Platform");
            }

            if (tmp != nullptr){
                GameServer::Instance() ->addPath(textureID, platform.image, DEFAULT_PLATFORM_PATH);
                tmp->init(platform.coordX + i * platformHeight, platform.coordY, textureID);

                actors.push_back(tmp);
                goPlatforms.push_back(tmp);
            }
        }
    }

    // Init Coins
    for(auto coin : levelConfig.coins) {
        for(long i = 0; i < coin.quantity; i++) {
            tmp = new Coin();
            if (tmp != nullptr){ //TODO we can initialice the Y position randomly too
                GameServer::Instance() ->addPath(COIN_ID, coin.image, DEFAULT_COIN_PATH);

                tmp->init(0, coin.coordY, COIN_ID);

                while(!checkCollision(goPlatforms, tmp)) {
                    tmp->setPosition(tmp->getXPosition() + 2, tmp->getYPosition());
                }

                actors.push_back(tmp);
                goCoins.push_back(tmp);
                Logger::getInstance()->debug("Coin created correctly");
            }
            else Logger::getInstance()->error("Error: couldn't create a Coin");
        }
    }

    // Init Holes
    for(auto xmlHole : levelConfig.holes) {
        Hole * h = new Hole();
        h->init(xmlHole.coordX, xmlHole.coordY, HOLE_ID);
        h->setDimensions(xmlHole.width, xmlHole.height);
        tmp = h;
        actors.push_back(tmp);
        Logger::getInstance()->debug("Hole created correctly");
    }

    // Init Pipes
    for(auto xmlPipe : levelConfig.pipes) {
        Pipe * p = new Pipe();
        p->init(xmlPipe.coordX, xmlPipe.coordY, PIPE_ID);
        tmp = p;
        GameServer::Instance() ->addPath(PIPE_ID, xmlPipe.image, DEFAULT_PIPE_PATH);
        actors.push_back(tmp);
        goPipes.push_back(tmp);
        Logger::getInstance()->debug("Pipe created correctly");
    }

    for(auto enemies : levelConfig.enemies) {
        for(long i = 0; i < enemies.quantity; i++) {
            if (enemies.type == ENEMY_TURTLE) {
                tmpEnemy = new EnemyTurtle();
                if (tmpEnemy != nullptr){
                    GameServer::Instance() ->addPath(KOOPA_GREEN_ID, enemies.image, DEFAULT_TURTLE_PATH);
                    tmpEnemy->init(900, 435, KOOPA_GREEN_ID, GameServer::Instance()->getCamera()->getCamera(),
                                       new EnemyMovement(0, 3));

                    Logger::getInstance()->debug("Turtle enemy created correctly");
                }
                else Logger::getInstance()->error("Error: couldn't create a Turtle Enemy");

            } else {
                tmpEnemy = new EnemyMushroom();
                if (tmpEnemy != nullptr){
                    GameServer::Instance() ->addPath(GOOMBA_ID, enemies.image, DEFAULT_MUSHROOM_PATH);
                    tmpEnemy->init(900, 425, GOOMBA_ID, GameServer::Instance()->getCamera()->getCamera(), new EnemyMovement(0, 5));

                    Logger::getInstance()->debug("Mushroom enemy created correctly");
                }
                else Logger::getInstance()->error("Error: couldn't create a Mushroom Enemy");
            }
            if (tmpEnemy != nullptr){
                tmp = tmpEnemy;

                while(!checkCollision(goPipes, tmp)) {
                    tmp->setPosition(tmp->getXPosition() + 2, tmp->getYPosition());
                }

                actors.push_back(tmp);
            }
        }
    }

    return actors;
}

std::vector<Player*>  Factory::createPlayers(std::vector<PlayerClient*> clients) {
    std::vector<Player*>  players;
    std::map<std::string, std::string> imgPaths = GameServer::Instance()->getPlayerPaths();
    auto it = imgPaths.begin();

    for(int i = 0; i < clients.size() && it != imgPaths.end(); i++, it++) {
        players.push_back(new Player(GameServer::Instance()->getCamera()->getCamera(), clients[i]->username, it->first));
        GameServer::Instance() ->addPath(it->first, it->second, DEFAULT_PLAYER_PATH);
        GameServer::Instance() ->addPath(it->first + "-big", "Sprites/Players/waluigi.png", "Sprites/Players/waluigi.png"); // ToDo Path for big character
        Logger::getInstance()->debug("Player " + clients[i]->username + " created correctly");
    }

    return players;
}

// Returns true if it's all clear, otherwise the goToInsert is colliding with some go in the vector
bool Factory::checkCollision(std::vector<GameObject *> gos, GameObject *goToInsert) {
    for(GameObject * go : gos) {
        if (CollisionsManager::Instance()->isInIntersection(go, goToInsert)) {
            return false;
        }
    }

    return true;
}

Factory::~Factory() = default;
