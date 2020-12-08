#ifndef TPTALLER1_GAMECLIENT_H
#define TPTALLER1_GAMECLIENT_H


#include <string>
#include <json.hpp>
#include <SDL2/SDL.h>
#include <src/BackgroundStages/BackgroundStage.h>
#include <src/Protocol/InitializeGameMsg.h>

#include "TextureManager.h"
#include "Camera.h"

class GameClient {
public:
    static GameClient* Instance();
    bool init(InitializeGameMsg initialize); //
    ~GameClient();
    void render();
    void update();
    bool createGameObjects(GameObjectsInit gameObjectsInit); //recibe la lista del init de game objects


private:
    //functions
    GameClient(); //Private constructor to prevent instancing.
    static GameClient* instance; //Here will be the instance stored.
    bool loadImages(std::map<std::string, std::vector<std::string>> imagePaths);
    bool loadTexts(StageInit stageInit);
    void createEnemie(GameObjectInit enemy, ObjectType enemyType);
    void createPlayer(GameObjectInit player);
    void createStaticObject(GameObjectInit gameObject, ObjectType objectType);


    SDL_Window* window;
    SDL_Renderer* renderer;
    BackgroundStage* background;

    bool playing = false;
    Camera* camera;
    Logger* logger = Logger::getInstance();
    TextureManager* textureManager;

    std::map<int, Player*> playersMap;
    std::map<int, GameObject*> gameObjectsMap;
};


#endif //TPTALLER1_GAMECLIENT_H
