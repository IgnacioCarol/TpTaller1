#include "GameServer.h"
#include "src/logger/logger.h"

GameServer* GameServer::instance = 0;

GameServer *GameServer::Instance() {
    //First time we create an instance of Game
    if(instance == 0) instance = new GameServer();
    return instance;
}

GameServer::GameServer() {

}

void GameServer::addPath(std::string ID, std::string imagePath, std::string defaultImagePath) {
    if (!imagePaths.count(ID)){
        imagePaths[ID].push_back(imagePath);
        imagePaths[ID].push_back(defaultImagePath);
    }
}

void GameServer::initializeGameObjects(int level) {
    gameObjects = factory->createGameObjectsFromLevelConfig(config->getLevel(level), SERVER);
}

SDL_Rect *  GameServer::getCamera() {
    return camera -> getCamera();
}

void GameServer::createGameObjects() {
    players = factory->createPlayersFromConfig(SERVER);
    initializeGameObjects(1);
}

bool GameServer::init(const char *levelName, std::string xmlPath) {
    config->load(xmlPath);

    window = config->getWindow();
    camera = new Camera(0, 0, window.width, window.height);

    logger -> info("Server init game success");
    playing = true;
    return true;
}

std::vector<std::string> GameServer::getPlayerPaths() {
    return playersPaths;
}

GameServer::~GameServer() {
//TODO VER QUÉ BORRA
}

