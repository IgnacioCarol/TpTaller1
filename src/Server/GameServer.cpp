#ifdef __APPLE__
#include "../CharacterStates/Normal.h"
#else
#include <src/CharacterStates/Normal.h>
#endif
#include "GameServer.h"

GameServer* GameServer::instance = nullptr;

GameServer *GameServer::Instance() {
    //First time we create an instance of Game
    if(instance == nullptr) instance = new GameServer();
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

    addPath("BG1", DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND, DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND);
    addPath("BG2", DEFAULT_STAGE_SECOND_LEVEL_BACKGROUND, DEFAULT_STAGE_SECOND_LEVEL_BACKGROUND);
    addPath("BG3", DEFAULT_STAGE_THIRD_LEVEL_BACKGROUND, DEFAULT_STAGE_THIRD_LEVEL_BACKGROUND);
    addPath("paused", "Sprites/Players/pausedPlayer.png","Sprites/Players/pausedPlayer.png");

    addSoundsPaths();
    initializeAllElementsOfGameServer(clients);
    json msg = getInitializationMsg();
    Server::getInstance()->broadcast(msg);

    logger -> info("Server init game success");
    playing = true;
    return true;
}

json GameServer::getInitializationMsg() {
    return ServerParser::buildGameInitMsg(getImagePaths(), soundsPath, getCamera(),
                                          this->stage, getGameObjects(), getPlayers());
}

int GameServer::getTimer() {
    return stage ? stage->getTimer()->getTimeSecond(): 0;
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
        player->changeState(new Normal(0, player->getFrameAmount()));
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
        if (player->getXPosition() >= stage->getLevelLimit() && player->getState() != "JUMPING"){
            player->changeState(new Paused(0, player->getFrameAmount(), false));
            changeLevelFlag = true;
        }
    }

    for (Player* player: players){
        changeLevelFlag &= (player->getState() == "FINISH" || player->getState() == "PAUSED");
    }

    if (changeLevelFlag) nextStage();
}

void GameServer::updateGameObjects() {
    for (GameObject* go: gameObjects){
        go->move();
    }
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

void GameServer::setChangeLevelFlag(bool setValue) {
    changeLevelFlag = setValue;
}
void GameServer::unpausePlayer(PlayerClient *playerClient) {
    for (Player *player: getPlayers()) {
        if (player->getUsername() == playerClient->username && player->getState() == "PAUSED") {
            Logger::getInstance()->info("Client " +  player->getUsername() + " is connected.");
            player->changeState(new Normal(0,player->getFrameAmount()));
        }
    }
}

void GameServer::pausePlayer(PlayerClient *playerClient) {
    for (Player * player: getPlayers()) {
        if (player->getUsername() == playerClient->username) {
            if (player->getState() != "PAUSED") {
                Logger::getInstance()->info("Client " + player->getUsername() + " is disconnected.");
                player->changeState(new Paused(0,player->getFrameAmount()));
            }
        }
    }
}

void GameServer::addSoundsPaths() {
    std::string path = "Sound_Effects/Sounds/";
    soundsPath["BUMP"] = path + "SuperMarioBrosSong.mp3";
    soundsPath["COIN"] = path + "Coin.mp3";
    soundsPath["GAME_OVER"] = path + "GameOver.mp3";
    soundsPath["HURRY_UP"] = path + "HurryUp.mp3";
    soundsPath["JUMP_MEDIUM"] = path + "JumpMedium.mp3";
    soundsPath["JUMP_SMALL"] = path + "JumpSmall.mp3";
    soundsPath["MARIO_DIES"] = path + "MarioDies.mp3";
    soundsPath["POWER_UP"] = path + "PowerUp.mp3";
    soundsPath["POWER_UP_APP"] = path + "PowerUpAppears.mp3";
    soundsPath["STOMP"] = path + "Stomp.mp3";
}