#ifndef TPTALLER1_GAME_H
#define TPTALLER1_GAME_H
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Camera.h"
#include "TextureManager.h"
#include "BackgroundStages/BackgroundStage.h"
#include "BackgroundStages/FirstStage.h"
#include "CharacterStates/EnemyMovement.h"
#include "Factory/Factory.h"
#include "Utils/Timer.h"
#include "config/Constants.h"
#include "config/Config.h"
#include "logger/logger.h"

#include "gameobjects/Player.h"


using namespace std;

class Game {
public:
    //Public instance function
    static Game* Instance();
    bool init(const char *levelName, int width, int height, std::string xmlPath);
    ~Game();

    bool init(const char *levelName, std::string xmlPath);
    bool loadImages();
    bool loadTexts();

    //Introduces the interactive objects in the game such as Mario, Koopa, etc.
    void createGameObjects();

    void render();

    void handleEvents();
    void update();
    void clean();
    bool isPlaying() const;

    void gameOver(){ playing = false;}

    void nextStage();
    void restartCharacters();

    SDL_Rect * getCamera();

private:
    Game(); //Private constructor to prevent instancing.
    static Game* instance; //Here will be the instance stored.

    void cleanGameObjects();
    void initializeGameObjects(int level);

    Camera* camera;
    Config* config = Config::getInstance();
    Factory* factory = Factory::getInstance();
    //Elements of the game
    Player* player;

    std::vector <Player*> players;
    std::vector <GameObject*> _gameObjects;

    bool playing = false;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TextureManager* textureManager;

    int lastValue;

    BackgroundStage* stage;
};


#endif //TPTALLER1_GAME_H
