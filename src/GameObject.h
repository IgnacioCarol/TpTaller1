#ifndef TPTALLER1_GAMEOBJECT_H
#define TPTALLER1_GAMEOBJECT_H

//Code to solve path conflicts between mac and ubuntu
#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#include "TextureManager.h"

#endif
#include <cstdio>
#include <string>

class GameObject {
public:
    virtual void init(int x, int y, std::string textureID, int currentFrame);
    virtual void draw(SDL_Renderer *renderer, int cameraX, int cameraY) = 0;

protected:
    //To manage movement
    int xPosition;
    int yPosition;

    //To manage the image
    size_t _width;
    size_t _height;
    TextureManager* textureManager = TextureManager::Instance();
    std::string _textureID;
    int _currentFrame;
};


#endif //TPTALLER1_GAMEOBJECT_H
