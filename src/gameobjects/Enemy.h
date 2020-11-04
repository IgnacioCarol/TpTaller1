#ifndef TPTALLER1_ENEMY_H
#define TPTALLER1_ENEMY_H

#include "GameObject.h"

const int enemyVelocity = 1;
class CharacterState;

class Enemy: public GameObject {
public:
    ~Enemy() override;
    virtual void init(size_t x, size_t y, std::string fileName, std::string defaultImg, std::string textureID, int currentFrame, SDL_Rect *camera, int framesAmount, CharacterState* state);
    void move() override;
    virtual void walk();  //If we have to implement the turtle that can fly we must add a new function fly()
    virtual void draw(SDL_Renderer *renderer, int cameraX, int cameraY, size_t width, size_t height);
    std::string getFilePath() override;
    std::string getID() override;
    std::string getDefault() override;

protected:
    CharacterState* enemyState{};
    int direction = -enemyVelocity;
    bool flipFlag = false;
    bool inScene = false; //To know if the enemy appears in scene
    SDL_Rect* cam{};
};


#endif //TPTALLER1_ENEMY_H
