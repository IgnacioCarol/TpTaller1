//
// Created by lisandro on 27/10/20.
//

#ifndef TPTALLER1_ENEMY_H
#define TPTALLER1_ENEMY_H
#include "GameObject.h"
const int enemyWidth = 400;
const int enemyHeight = 400;
const int enemyVelocity = 1;
class CharacterState;

class Enemy: public GameObject {
public:
    Enemy();
    void init(size_t x, size_t y, std::string textureID, int currentFrame, SDL_Rect *camera, int framesAmount, CharacterState* state);
    void move() override;
    void walk();  //If we have to implement the turtle that can fly we must add a new function fly()
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;

protected:
    CharacterState* enemyState;
    int direction = -enemyVelocity;
    bool flipFlag = false;
    bool inScene = false; //To know if the enemy appears in scene
    SDL_Rect* cam;
};


#endif //TPTALLER1_ENEMY_H
