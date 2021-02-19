#ifndef TPTALLER1_ENEMYMUSHROOM_H
#define TPTALLER1_ENEMYMUSHROOM_H

#include "Enemy.h"

#define POINTS_MUSHROOM 500

class CharacterState;

class EnemyMushroom : public Enemy{
public:
    void init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, CharacterState *state) override;
    int getHeight() override;
    int getWidth() override;
    void move() override; // override;
    void walk() override;  //If we have to implement the turtle that can fly we must add a new function fly()
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY, size_t width, size_t height) override;

    int getFloorPosition() override;
private:
    //Related to the image
    static const int ENEMY_WIDTH = 400;

    static const int ENEMY_HEIGHT = 400;

    int centerXPos() override;
};


#endif //TPTALLER1_ENEMYMUSHROOM_H
