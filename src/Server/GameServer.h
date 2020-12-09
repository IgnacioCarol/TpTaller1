#ifndef TPTALLER1_GAMESERVER_H
#define TPTALLER1_GAMESERVER_H


#include <string>
#include "../config/Config.h"
#include "../Camera.h"
#include "../config/Constants.h"
#include "../gameobjects/GameObject.h"
#include "../gameobjects/Player.h"
#include "../Factory/Factory.h"
#include "../BackgroundStages/BackgroundStage.h"
#include "../logger/logger.h"
#include "ServerParser.h"
#include "Server.h"
#include "../BackgroundStages/FirstStage.h"


class GameServer {
public:
    static GameServer* Instance();
    bool init(std::vector<PlayerClient*> clients);

    //Getters to GameServer purpose
    Camera *  getCamera();
    std::map<std::string, std::string>  getPlayerPaths();

    //Getters to send info to the client
    std::map<std::string, std::vector<std::string>> getImagePaths();
    std::vector <GameObject*> getGameObjects();
    std::vector <Player*> getPlayers();

    //Methods to control images/level flow
    void addPath(std::string ID, std::string imagePath, std::string defaultImagePath);
    void nextStage();
    void restartCharacters();

    //Methods to control game flow
    void sendInitializationMsg();
    bool isPlaying() const;
    void cleanGameObjects();
    void handleEvents(); //TODO: ver qué debería devolver (porque puede cambiar de escena/terminar juego)

    ~GameServer();

    int getTimer();

    void updatePlayers();

    BackgroundStage* getBackgroundStage();

    bool changeLevel();

    void gameOver();

    void setChangeLevelFlag(bool setValue);

private:
    GameServer(); //Private constructor to prevent instancing.
    static GameServer* instance; //Here will be the instance stored.
    void initializeGameObjects(int level);
    void initializeAllElementsOfGameServer(std::vector<PlayerClient*> clients);


    //Utils
    Logger* logger = Logger::getInstance();
    Config* config = Config::getInstance();
    Factory* factory = Factory::getInstance();
    Camera* camera;
    Window window;

    //Information about the game
    std::map<std::string, std::string> playersPaths = {{"player0", PLAYER_0_IMG_PATH},
                                                       {"player1", PLAYER_1_IMG_PATH},
                                                       {"player2", PLAYER_2_IMG_PATH},
                                                       {"player3", PLAYER_3_IMG_PATH}};
    std::map<std::string, std::vector<std::string>> imagePaths;
    BackgroundStage* stage;

    //Elements of the game
    std::vector <GameObject*> gameObjects;
    std::vector <Player*>  players;

    bool playing = false;
    bool changeLevelFlag = false;
    const int LEVEL_LIMIT = 3100; //ToDo chequear que este bien
};


#endif //TPTALLER1_GAMESERVER_H
