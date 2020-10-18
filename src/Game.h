#ifndef TPTALLER1_GAME_H
#define TPTALLER1_GAME_H

#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include <vector>
#include "Stage.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"

#define TITLEMSG "Super Mario Bros"

class Game {
public:

    //Public instance function
    static Game* Instance();
    //Loads the initial scene and player according to XML's specifications
    //If something's missing, it will show the default settings
    //If something fails, it throws an exception
    void init();

    //Returns 1 is the game is running
    //0 otherwise
    int isRunning();

    void handleUserEvents();

    void updateState();

    void render();

    void destroy();

private:
    Game(); //private constructor
    static Game* _instance; //_Instance

    int _running;
  //  Stage* stage;
    std::vector<GameObject*> _gameObjects;
};


#endif //TPTALLER1_GAME_H
