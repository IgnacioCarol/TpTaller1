#ifndef TPTALLER1_GAME_H
#define TPTALLER1_GAME_H
#include <iostream>
#include <SDL2/SDL.h>
#include "Camera.h"
#include <string>
#include "TextureManager.h"
#include "GameObject.h"
#include "Player.h"

using namespace std;

class Game {
public:
    //Public instance function
    static Game* Instance();

    bool init(const char* levelName, int width, int height);
    bool loadImages();

    //Introduces the interactive objects in the game such as Mario, Koopa, etc.
    void createGameObjects();

    void render();
    //void update(){}
    void handleEvents();
    void clean();
    bool isPlaying() const{ return playing;}

    void gameOver(){ playing = false;}

    void nextStage();

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

    int lastValue;

    bool setBackground(const char *path);
    Stage* stage;
};


#endif //TPTALLER1_GAME_H
