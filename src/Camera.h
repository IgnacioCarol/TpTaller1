#ifndef TPTALLER1_CAMERA_H
#define TPTALLER1_CAMERA_H

#include <iostream>
#include <SDL2/SDL.h>
// #include "Game.h"


class Camera {

public:
    Camera(int x, int y, int width, int height);
    ~Camera() = default;


    void update(int playerXPos, int borderLimit);

    int lastValue = 0;

    SDL_Rect *getCamera();

    int getXpos();

    void restartPos();

    void setXPos(int newX);

private:
    SDL_Rect camera{};
};


#endif //TPTALLER1_CAMERA_H
