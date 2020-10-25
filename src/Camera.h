//
// Created by nacho on 25/10/20.
//
#include <iostream>
#include <SDL2/SDL.h>
#ifndef TPTALLER1_CAMERA_H
#define TPTALLER1_CAMERA_H


class Camera {

public:
    Camera(int x, int y, int width, int height);
    SDL_Rect camera{};

    void render(int playerXPos, int borderLimit);

    int lastValue = 0;

    SDL_Rect *getCamera();

    int getXpos();
};


#endif //TPTALLER1_CAMERA_H
