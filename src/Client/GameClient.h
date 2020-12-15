#ifndef TPTALLER1_GAMECLIENT_H
#define TPTALLER1_GAMECLIENT_H


#include <string>
#include <json.hpp>
#include <SDL2/SDL.h>
#include "../BackgroundStages/BackgroundStage.h"
#include "../Protocol/GameMsgParams.h"
#include "../gameobjects/GameObject.h"

#include "../TextureManager.h"
#include "../Camera.h"
#include "../gameobjects/Enemy.h"
#include "../gameobjects/EnemyTurtle.h"
#include "../gameobjects/PlatformNormal.h"
#include "../gameobjects/PlatformSurprise.h"
#include "../gameobjects/Coin.h"
#include "../gameobjects/EnemyMushroom.h"
#include "../CharacterStates/EnemyMovement.h"
#include "../CharacterStates/Normal.h"

class GameClient {
public:
    static GameClient* Instance();
    bool init(GameMsgParams initialize, const char* username); //
    ~GameClient();
    void render();
    void update(GameMsgPlaying initialize);
    bool createGameObjects(GameObjectsInit gameObjectsInit);
    bool isPlaying();
    void gameOver();
    void changeLevel(GameMsgLevelChange nextLevelConfig);
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
    void updateGameObjects(std::vector<GameObjectPlaying>);
    void changeLevelBackground(StageInit nextLevelConfig);
    void renderPlayers();


    SDL_Window* window;
    SDL_Renderer* renderer;
    BackgroundStage* background;

    bool playing = true;
    Camera* camera;
    Logger* logger = Logger::getInstance();
    TextureManager* textureManager;
    std::string clientUsername;

    std::map<int, Player*> playersMap;
    std::map<int, GameObject*> gameObjectsMap;
    std::vector<int> idsToRender;
};


#endif //TPTALLER1_GAMECLIENT_H
