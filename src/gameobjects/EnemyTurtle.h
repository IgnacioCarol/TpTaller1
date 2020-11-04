#ifndef TPTALLER1_ENEMYTURTLE_H
#define TPTALLER1_ENEMYTURTLE_H

#include "Enemy.h"

#define etWidth 450
#define etHeight 450
#define etID "koopaGreen" //Enemy turtle
#define defaultKoopa "Sprites/Default/defaultKoopa.png"

class EnemyTurtle : public Enemy{
public:
    void init(size_t x, size_t y, std::string fileName, std::string defaultImg, std::string textureID, int currentFrame, SDL_Rect *camera, int framesAmount, CharacterState* state) override;
    void walk() override;  //If we have to implement the turtle that can fly we must add a new function fly()
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY, size_t width, size_t height) override;

};


#endif //TPTALLER1_ENEMYTURTLE_H
