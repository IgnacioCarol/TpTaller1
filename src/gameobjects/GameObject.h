#ifndef TPTALLER1_GAMEOBJECT_H
#define TPTALLER1_GAMEOBJECT_H

//Code to solve path conflicts between mac and ubuntu
#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "../TextureManager.h"
#include <cstdio>
#include <string>

class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void init(int x, int y, std::string textureID);
    virtual void draw(SDL_Renderer *renderer, int cameraX, int cameraY) = 0;
    virtual void move();
    virtual int getHeight();
    virtual void setPosition(int x, int y);
    virtual void setDirection(int direction);
    virtual void setState(std::string state);

protected:
    //To manage the movement
    int xPosition;
    int yPosition;

    //To manage the image
    TextureManager* textureManager = TextureManager::Instance();
    std::string _textureID;
    int _currentFrame;
};


#endif //TPTALLER1_GAMEOBJECT_H
