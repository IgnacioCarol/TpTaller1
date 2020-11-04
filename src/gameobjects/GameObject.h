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
    virtual void init(int x, int y, std::string fileName, std::string defaultImg,std::string textureID, int currentFrame);
    virtual void draw(SDL_Renderer *renderer, int cameraX, int cameraY) = 0;
    virtual std::string getFilePath();
    virtual std::string getID();
    virtual std::string getDefault();
    virtual void move();

protected:
    //To manage the movement
    int xPosition;
    int yPosition;

    //To manage the image
    TextureManager* textureManager = TextureManager::Instance();
    std::string _textureID;
    std::string _fileName;
    std::string _defaultImg;
    int _currentFrame;
};


#endif //TPTALLER1_GAMEOBJECT_H
