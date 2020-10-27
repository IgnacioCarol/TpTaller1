//
// Created by lisandro on 27/10/20.
//

#ifndef TPTALLER1_ENEMY_H
#define TPTALLER1_ENEMY_H
class CharacterState;


#include "GameObject.h"

class Enemy: public GameObject {
public:
    void init(size_t x, size_t y, std::string textureID, int currentFrame, SDL_Rect *camera, int framesAmount, CharacterState* state);
    void move();
    void walk();  //If we have to implement the turtle that can fly we must add a new function fly()
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY);

protected:
    CharacterState* enemyState;
};


#endif //TPTALLER1_ENEMY_H
