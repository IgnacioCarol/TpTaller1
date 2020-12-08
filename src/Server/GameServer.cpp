#include "GameServer.h"

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
    gameObjects = factory->createGameObjectsFromLevelConfig(config->getLevel(level));
}

Camera *  GameServer::getCamera() {
    return camera;
}

void GameServer::initializeAllElementsOfGameServer() {
    players = factory->createPlayers();
    initializeGameObjects(1);
}

bool GameServer::init() {
    window = config->getWindow();
    camera = new Camera(0, 0, window.width, window.height);
    initializeAllElementsOfGameServer();

    logger -> info("Server init game success");
    playing = true;

    sendInitializationMsg();

    return true;
}

void GameServer::sendInitializationMsg() {
    json msg = ServerParser::buildGameInitMsg(getImagePaths(), getCamera(), this->stage, getGameObjects(), getPlayers());
}

std::vector<std::string> GameServer::getPlayerPaths() {
    return playersPaths;
}

void GameServer::cleanGameObjects() {
    for(std::vector<GameObject*>::size_type i = 0; i != gameObjects.size(); i++) {
        delete gameObjects[i];
    }
    gameObjects.clear();
}

GameServer::~GameServer() {
    map<string, Player*>::iterator it;
    for (auto& player: players) {
        delete player;
    }
    cleanGameObjects(); //TODO: OJO, ACA USO CLEANOBJECTS PERO EN EL ORIGINAL USAN CICLO -> PREGUNTAR
    Logger::getInstance()->info("All Game Objects were deleted");
    delete this->camera;
    Logger::getInstance()->info("The camera was deleted");
    delete this->config;
    Logger::getInstance()->info("The parser(config) was deleted");
    delete this->factory;
    Logger::getInstance()->info("The Factory was deleted");
}

void GameServer::handleEvents() {
    for (auto & player : players) {
        player->move(); //TODO: acá debería recibir los mensajes del client
    }
}

void GameServer::nextStage() {
    BackgroundStage *currentStage = this->stage;
    stage = stage->nextStage();
    if (currentStage != stage) {
        Logger::getInstance()->info("Stage changed");
        cleanGameObjects();
        initializeGameObjects(stage->getLevel());
    }
    delete currentStage;
}

void GameServer::restartCharacters() {
    Logger::getInstance()->info("Restarting Player and Camera position");
    for (auto & player : players) {
        player->restartPos(0, 380);
    }
    camera->restartPos();
}

bool GameServer::isPlaying() const {
    return this->playing && !this->stage->isTimeOver();
}

std::map<std::string, std::vector<std::string>> GameServer::getImagePaths() {
    return imagePaths;
}

std::vector<GameObject *> GameServer::getGameObjects() {
    return gameObjects;
}

std::vector<Player *> GameServer::getPlayers() {
    return players;
}
