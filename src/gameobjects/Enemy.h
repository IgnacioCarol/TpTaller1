#ifndef TPTALLER1_ENEMY_H
#define TPTALLER1_ENEMY_H

#include "GameObject.h"
#include "Player.h"
#include "../CharacterStates/Dying.h"


class CharacterState;
class Player;

class Enemy: public GameObject {
public:
    ~Enemy() override;
    virtual void init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, CharacterState *state);
    void move() override;
    void die() override;
    void dieFalling() override;
    void fall() override;
    virtual void walk();  //If we have to implement the turtle that can fly we must add a new function fly()
    virtual void draw(SDL_Renderer *renderer, int cameraX, int cameraY, size_t width, size_t height);
    void setPosition(int x, int y) override;
    void setDirection(bool direction) override;
    bool getDirection() override;
    void setState(std::string state) override;
    std::string getState() override;

    int getFrameAmount() override;


    size_t getPoints() override;

protected:
    static const int enemyVelocity = 1;
    static const int GRAVITY_ENEMY = 3;
    CharacterState* enemyState{};
    int direction = -enemyVelocity;
    bool flipFlag = false;
    SDL_Rect* cam{};

    size_t pointsToPlayer;

    void collideWith(GameObject *go) override;

    bool falling = false;
};


#endif //TPTALLER1_ENEMY_H
