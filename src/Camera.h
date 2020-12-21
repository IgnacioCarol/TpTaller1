#ifndef TPTALLER1_CAMERA_H
#define TPTALLER1_CAMERA_H

#include "gameobjects/Player.h"

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>


class Camera {

public:
    Camera(int x, int y, int width, int height);
    ~Camera() = default;


    void update(std::vector<Player *> players);

    int lastValue = 0;

    SDL_Rect *getCamera();

    int getXpos();

    void restartPos();

    void setXPos(int newX);

private:
    SDL_Rect camera{};
    static const int displacement = 300;
};


#endif //TPTALLER1_CAMERA_H
