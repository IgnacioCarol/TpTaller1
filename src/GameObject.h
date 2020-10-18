#ifndef TPTALLER1_GAMEOBJECT_H
#define TPTALLER1_GAMEOBJECT_H

//Code to solve path conflicts between mac and ubuntu
#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include <cstdio>
#include <string>

class GameObject {
public:
    virtual void init(size_t x, size_t y, size_t width, size_t height, std::string textureID, size_t frameAmount, currentFrame, currentRow);
    virtual void draw();
    virtual void update() = 0;
    virtual void clean();

protected:
    size_t _x;
    size_t _y;

    size_t _width;
    size_t _height;

    std::string _textureID;

    int _currentFrame;
    int _currentRow;

    size_t _frameAmount; //amount of frames in sprite sheet

};


#endif //TPTALLER1_GAMEOBJECT_H
