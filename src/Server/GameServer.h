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


class GameServer {
public:
    static GameServer* Instance();
    bool init();

    //Getters to GameServer purpose
    SDL_Rect *  getCamera();
    std::vector<std::string>  getPlayerPaths();

    //Getters to send info to the client
    std::map<std::string, std::vector<std::string>> getImagePaths();
    std::vector <GameObject*> getGameObjects();
    std::map<std::string, Player*> getPlayers();

    //Methods to control images/level flow
    void addPath(std::string ID, std::string imagePath, std::string defaultImagePath);
    void nextStage();
    void restartCharacters();

    //Methods to control game flow
    bool isPlaying() const;
    void cleanGameObjects();
    void handleEvents(); //TODO: ver qué debería devolver (porque puede cambiar de escena/terminar juego)

    ~GameServer();

private:
    GameServer(); //Private constructor to prevent instancing.
    static GameServer* instance; //Here will be the instance stored.
    void initializeGameObjects(int level);
    void initializeAllElementsOfGameServer();


    //Utils
    Logger* logger = Logger::getInstance();
    Config* config = Config::getInstance();
    Factory* factory = Factory::getInstance();
    Camera* camera;
    Window window;

    //Information about the game
    std::vector <std::string> playersPaths = {PLAYER_0_IMG_PATH,
                                             PLAYER_1_IMG_PATH,
                                             PLAYER_2_IMG_PATH,
                                             PLAYER_3_IMG_PATH};
    std::map<std::string, std::vector<std::string>> imagePaths;
    BackgroundStage* stage;

    //Elements of the game
    std::vector <GameObject*> gameObjects;
    std::map<std::string, Player*>  players;

    bool playing = false;
};


#endif //TPTALLER1_GAMESERVER_H
