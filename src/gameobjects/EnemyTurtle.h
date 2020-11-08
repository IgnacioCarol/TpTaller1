#ifndef TPTALLER1_ENEMYTURTLE_H
#define TPTALLER1_ENEMYTURTLE_H

#include "Enemy.h"

#define etWidth 360
#define etHeight 360

class EnemyTurtle : public Enemy{
public:
    void init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, CharacterState *state) override;
    void walk() override;  //If we have to implement the turtle that can fly we must add a new function fly()
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY, size_t width, size_t height) override;

};


#endif //TPTALLER1_ENEMYTURTLE_H
