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
    gameObjects = factory->createGameObjectsFromLevelConfig(config->getLevel(level));
}

SDL_Rect *  GameServer::getCamera() {
    return camera -> getCamera();
}

void GameServer::initializeAllElementsOfGameServer() {
    players = factory->createPlayersFromConfig();
    initializeGameObjects(1);
}

bool GameServer::init(const char *levelName, std::string xmlPath) {
    config->load(xmlPath);

    window = config->getWindow();
    camera = new Camera(0, 0, window.width, window.height);
    initializeAllElementsOfGameServer();

    logger -> info("Server init game success");
    playing = true;
    return true;
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
    for (it = players.begin(); it != players.end(); it++ )
    {
        delete it->second;
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
    for (auto & player : players)
    {
        player.second->move(); //TODO: acá debería recibir los mensajes del client
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
    for (auto & player : players)
    {
        player.second->restartPos(0, 380);
    }
    camera->restartPos();
}

bool GameServer::isPlaying() const {
    return this->playing && !this->stage->isTimeOver();
}
