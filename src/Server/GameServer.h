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
#include "../Utils/Score.h"
#include <unordered_set>

class ServerParser;
class Camera;
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
    json getInitializationMsg();
    bool isPlaying() const;
    void cleanGameObjects();

    void updateGameObjectsOnScreen();
    std::vector <GameObject*> getGameObjectsOnScreen();
    ~GameServer();

    void unpausePlayer(PlayerClient *player);

    void pausePlayer(PlayerClient *player);

    int getTimer();

    void updatePlayers();

    void updateGameObjects();

    BackgroundStage* getBackgroundStage();

    bool changeLevel();

    void gameOver();

    void setChangeLevelFlag(bool setValue);

    void deleteGameObject(GameObject *pObject);
    void addSoundsPaths();

    bool arePlayersAlive() const;

    bool shouldSendScore();

    Score* getScore();

    void updateSendScore();
    bool isTimeOver();

private:
    GameServer(); //Private constructor to prevent instancing.
    static GameServer* instance; //Here will be the instance stored.
    void initializeGameObjects(int level);
    void initializeAllElementsOfGameServer(std::vector<PlayerClient*> clients);
    //Utils
    Logger* logger = Logger::getInstance();
    Config* config = Config::getInstance();
    Factory* factory = Factory::getInstance();
    Score* score = Score::getInstance();
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
    unordered_set <GameObject*> gameObjectsDeleted;
    std::vector <Player*>  players;
    bool playing = false;
    bool changeLevelFlag = false;
    std::vector <GameObject*> gameObjectsOnScreen;
    bool sendScore = false;

    int currentRaceIndex = 0;
    const int levelRacePoints[4] = {2500, 1500, 1000, 500};

    std::map<std::string, std::string> soundsPath;
};


#endif //TPTALLER1_GAMESERVER_H
