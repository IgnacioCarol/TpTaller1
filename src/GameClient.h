#ifndef TPTALLER1_GAMECLIENT_H
#define TPTALLER1_GAMECLIENT_H


#include <string>
#include <json.hpp>
#include <SDL2/SDL.h>
#include "BackgroundStages/BackgroundStage.h"
#include "Protocol/GameMsgParams.h"
#include "gameobjects/GameObject.h"

#include "TextureManager.h"
#include "Camera.h"

class GameClient {
public:
    static GameClient* Instance();
    bool init(GameMsgParams initialize); //
    ~GameClient();
    void render();
    void update(GameMsgPlaying initialize); //ToDo por ahora digo que recibe esto para orientarme despues poner estructura correcta
    bool createGameObjects(GameObjectsInit gameObjectsInit); //recibe la lista del init de game objects
    bool isPlaying();
    void gameOver();
    void changeLevel(GameObjectsInit nextLevelConfig);
    void clean();

private:
    //functions
    GameClient(); //Private constructor to prevent instancing.
    static GameClient* instance; //Here will be the instance stored.
    bool loadImages(std::map<std::string, std::vector<std::string>> imagePaths);
    bool loadTexts(bool isDefault, std::vector<GameObjectInit> players);
    void createEnemy(GameObjectInit enemy, GameObjectType enemyType);
    void createPlayer(GameObjectInit player);
    void createStaticObject(GameObjectInit gameObject, GameObjectType objectType);
    void initBackground(SDL_Renderer* renderer, StageInit stage);
    void updatePlayers(std::vector<GamePlayerPlaying>);
    void changeLevelBackground();


    SDL_Window* window;
    SDL_Renderer* renderer;
    BackgroundStage* background;

    bool playing = true;
    Camera* camera;
    Logger* logger = Logger::getInstance();
    TextureManager* textureManager;

    std::map<int, Player*> playersMap;
    std::map<int, GameObject*> gameObjectsMap;
};


#endif //TPTALLER1_GAMECLIENT_H
