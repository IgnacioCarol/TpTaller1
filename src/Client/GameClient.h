#ifndef TPTALLER1_GAMECLIENT_H
#define TPTALLER1_GAMECLIENT_H


#include <string>
#include <json.hpp>
#include <SDL2/SDL.h>
#include "../BackgroundStages/BackgroundStage.h"
#include "../Protocol/GameMsgParams.h"
#include "../gameobjects/GameObject.h"
#include "../config/Constants.h"
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
#include "../Utils/MusicManager.h"

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
    void stopShowPartialScore();
    void showPartialScore(GameMsgShowPartialScore params);
    void clean();
    void setServerDown();
    void pauseSoundEffects(int music, int sounds);
private:
    //functions
    GameClient(); //Private constructor to prevent instancing.
    static GameClient* instance; //Here will be the instance stored.
    bool loadImages(std::map<std::string, std::vector<std::string>> imagePaths);
    void loadSounds(std::map<std::string, std::string> soundPaths);
    bool loadTexts(bool isDefault, std::vector<GameObjectInit> players);
    void createEnemy(GameObjectInit enemy, GameObjectType enemyType);
    void createPlayer(GameObjectInit player);
    void createStaticObject(GameObjectInit gameObject, GameObjectType objectType);
    void initBackground(SDL_Renderer* renderer, StageInit stage);
    void updatePlayers(std::vector<GamePlayerPlaying>);
    void updateGameObjects(std::vector<GameObjectPlaying>);
    void changeLevelBackground(StageInit nextLevelConfig);
    void renderPlayers();
    void renderPointsAndLives(int yPosition, int points, int lives);
    void renderPartialScore();


    SDL_Window* window;
    SDL_Renderer* renderer;
    BackgroundStage* background;

    bool playing = true;
    bool serverIsDown = false;
    Camera* camera;
    Logger* logger = Logger::getInstance();
    TextureManager* textureManager;
    MusicManager* musicManager;
    std::string clientUsername;

    std::map<int, Player*> playersMap;
    std::map<int, GameObject*> gameObjectsMap;
    std::vector<int> idsToRender;
    int levelLimit;
    bool levelCompleted = false;
    bool showScore = false;

    static const int DIGITS = 6;

    void loadScoreText();
};


#endif //TPTALLER1_GAMECLIENT_H
