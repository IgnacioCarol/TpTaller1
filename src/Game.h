#ifndef TPTALLER1_GAME_H
#define TPTALLER1_GAME_H
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Camera.h"
#include "TextureManager.h"
#include "gameobjects/GameObject.h"
#include "gameobjects/Player.h"
#include "Utils/Timer.h"
#include "BackgroundStages/BackgroundStage.h"
#include "config/Constants.h"
#include "src/Camera.h"

#include "gameobjects/EnemyMushroom.h"


using namespace std;

class Game {
public:
    //Public instance function
    static Game* Instance();

    bool init(const char* levelName, int width, int height);
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
    ~Game();
    static Game* instance; //Here will be the instance stored.
    Camera* camera;
    //Elements of the game
    Player* player;

    std::vector <GameObject*> _gameObjects;

    bool playing = false;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TextureManager* textureManager = TextureManager::Instance();
    Printer* printer = Printer::getInstance();

    int lastValue;

    BackgroundStage* stage;
};


#endif //TPTALLER1_GAME_H
