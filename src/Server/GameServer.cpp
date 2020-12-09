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

void GameServer::initializeAllElementsOfGameServer(std::vector<PlayerClient*> clients) {
    players = factory->createPlayers(clients);
    initializeGameObjects(1);
}

bool GameServer::init(std::vector<PlayerClient*> clients) {
    window = config->getWindow();
    camera = new Camera(0, 0, window.width, window.height);
    stage = new FirstStage();
    initializeAllElementsOfGameServer(clients);
    sendInitializationMsg();

    logger -> info("Server init game success");
    playing = true;
    return true;
}

void GameServer::sendInitializationMsg() {
    addPath("BG1", DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND, DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND);
    addPath("BG2", DEFAULT_STAGE_SECOND_LEVEL_BACKGROUND, DEFAULT_STAGE_SECOND_LEVEL_BACKGROUND);
    addPath("BG3", DEFAULT_STAGE_THIRD_LEVEL_BACKGROUND, DEFAULT_STAGE_THIRD_LEVEL_BACKGROUND);
    addPath("paused", "Sprites/Players/pausedPlayer.png","Sprites/Players/pausedPlayer.png");
    json msg = ServerParser::buildGameInitMsg(getImagePaths(), getCamera(), this->stage, getGameObjects(), getPlayers());
    Server::getInstance()->broadcast(msg);
}

int GameServer::getTimer() {
    return stage->getTimer()->getTimeSecond();
}

std::map<std::string, std::string> GameServer::getPlayerPaths() {
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
        player->move(std::vector<int>()); //TODO: acá debería recibir los mensajes del client
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

void GameServer::updatePlayers() {
    for (Player* player: players) {
        player->move();
        if (player->getXPosition() >= LEVEL_LIMIT && player->getState() != "JUMPING"){
            player->changeState(new Paused(0, player->getFrameAmount()));
            changeLevelFlag = true;
        }
    }

    for (Player* player: players){
        changeLevelFlag &= (player->getState() == "DONE" || player->getState() == "PAUSED");
    }

    if (changeLevelFlag) nextStage();
}

BackgroundStage *GameServer::getBackgroundStage() {
    return stage;
}

bool GameServer::changeLevel() {
    return changeLevelFlag;
}

void GameServer::gameOver() {
    this->playing = false;
}
