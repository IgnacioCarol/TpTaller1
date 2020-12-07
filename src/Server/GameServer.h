#ifndef TPTALLER1_GAMESERVER_H
#define TPTALLER1_GAMESERVER_H


#include <string>
#include <src/config/Config.h>
#include "Camera.h"
#include "src/config/Constants.h"


class GameServer {
public:
    static GameServer* Instance();
    bool init(const char *levelName, std::string xmlPath);

    //Getters
    SDL_Rect *  getCamera();
    std::vector<std::string>  getPlayerPaths();

    //Methods to control images/level flow
    void addPath(std::string ID, std::string imagePath, std::string defaultImagePath);
    void nextStage();
    void restartCharacters();

    //Methods to control game flow
    bool isPlaying() const;
    void cleanGameObjects();
    void handleEvents();

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
