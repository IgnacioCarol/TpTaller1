#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"
#ifndef TPTALLER1_CAMERA_H
#define TPTALLER1_CAMERA_H


class Camera {

public:
    Camera(int x, int y, int width, int height);


    void render(int playerXPos, int borderLimit);

    int lastValue = 0;

    SDL_Rect *getCamera();

    int getXpos();

    void restartPos();

private:
    SDL_Rect camera{};
};


#endif //TPTALLER1_CAMERA_H
