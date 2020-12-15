#ifndef TPTALLER1_GAMEOBJECT_H
#define TPTALLER1_GAMEOBJECT_H

//Code to solve path conflicts between mac and ubuntu
#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "../TextureManager.h"
#include "../Utils/IdGenerator.h"
#include <cstdio>
#include <string>

enum GameObjectType {
    GOT_COIN,
    GOT_ENEMY_MUSHROOM,
    GOT_ENEMY_TURTLE,
    GOT_PLATFORM_NORMAL,
    GOT_PLATFORM_SURPRISE,
    GOT_PLAYER
};

class GameObject {
public:
    GameObject();
    virtual ~GameObject() = default;
    virtual void init(int x, int y, std::string textureID);
    virtual void draw(SDL_Renderer *renderer, int cameraX, int cameraY) = 0;
    virtual void move();
    virtual int getHeight();
    virtual int getFrameAmount();
    int getId();
    GameObjectType getType();
    std::string getTextureId();
    int getXPosition();
    int getYPosition();
    virtual void setPosition(int x, int y);
    virtual bool getDirection();
    virtual void setDirection(bool direction);
    virtual void setState(std::string state);
    virtual std::string getState();
    bool isAtScene(int camXPos);

protected:
    //To manage the movement
    int xPosition;
    int yPosition;

    //To manage the image
    TextureManager* textureManager = TextureManager::Instance();
    std::string _textureID;
    int _currentFrame;
    bool atScene = false; //To know if the object its at scene

    int id;
    GameObjectType type;
};

#endif //TPTALLER1_GAMEOBJECT_H
