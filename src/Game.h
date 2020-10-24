//
// Created by lisandro on 23/10/20.
//

#ifndef TPTALLER1_GAME_H
#define TPTALLER1_GAME_H
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "TextureManager.h"
#include "Mario.h"
#include "Hongo.h"

class Game {
public:
    Game() {}
    ~Game() {}
    bool init(const char* levelName, int width, int height);
    bool loadImages();
    void render();
    //void update(){}
    void handleEvents();
    void clean();
    bool isPlaying(){ return playing;}

    void setGameObjects(Mario* mario, Hongo* hongo){
        this->mario = mario;
        this->hongo = hongo;
    }

    void gameOver(){ playing = false;}

private:
    bool playing = false;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TextureManager* textureManager = TextureManager::Instance();
    Mario* mario;
    Hongo* hongo;
};


#endif //TPTALLER1_GAME_H
