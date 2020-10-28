#ifndef TPTALLER1_GAME_H
#define TPTALLER1_GAME_H
#include <iostream>
#include <SDL2/SDL.h>
#include "Camera.h"
#include <string>
#include "TextureManager.h"
#include "gameobjects/GameObject.h"
#include "gameobjects/Player.h"
#include "../src/BackgroundStages/BackgroundStage.h"

#define TEXT_WORLD_LEVEL_LABEL_KEY "world_level"
#define TEXT_WORLD_LEVEL_LABEL_VALUE "WORLD"
#define TEXT_WORLD_LEVEL_NUMBER_KEY "world_level_number"


using namespace std;

class Game {
public:
    //Public instance function
    static Game* Instance();

    bool init(const char* levelName, int width, int height);
    bool loadImages();
    bool loadLevel(int level);

    //Introduces the interactive objects in the game such as Mario, Koopa, etc.
    void createGameObjects();

    void render();
    //void update(){}
    void handleEvents();
    void clean();
    bool isPlaying() const{ return playing;}

    void gameOver(){ playing = false;}

    void nextStage();
    void restartCharacters();

private:
    Game(); //Private constructor to prevent instancing.
    static Game* instance; //Here will be the instance stored.
    Camera* camera;
    //Elements of the game
    Player* player;
    //std::vector <GameObject*> _gameObjects;

    bool playing = false;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TextureManager* textureManager = TextureManager::Instance();
    Printer* printer = Printer::getInstance();

    int lastValue;

    BackgroundStage* stage;
};


#endif //TPTALLER1_GAME_H
