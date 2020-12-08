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
    void update(InitializeGameMsg initialize); //ToDo por ahora digo que recibe esto para orientarme despues poner estructura correcta
    bool createGameObjects(GameObjectsInit gameObjectsInit); //recibe la lista del init de game objects
    bool isPlaying();
    void clean();

private:
    //functions
    GameClient(); //Private constructor to prevent instancing.
    static GameClient* instance; //Here will be the instance stored.
    bool loadImages(std::map<std::string, std::vector<std::string>> imagePaths);
    bool loadTexts();
    void createEnemy(GameObjectInit enemy, ObjectType enemyType);
    void createPlayer(GameObjectInit player);
    void createStaticObject(GameObjectInit gameObject, ObjectType objectType);
    void initBackground(SDL_Renderer* renderer, StageInit stage);
    void updatePlayers(GameObjectsInit initialize);


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
