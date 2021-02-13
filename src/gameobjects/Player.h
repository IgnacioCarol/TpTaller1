#ifndef TPTALLER1_MARIO_H
#define TPTALLER1_MARIO_H


#include <string>
#include "../logger/logger.h"
#include "../TextureManager.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Coin.h"
#include "PlatformNormal.h"
#include "../Utils/CollisionsManager.h"
#include "../Utils/MusicManager.h"
#include <cstdio>
#include <utility>
#include "../CharacterStates/Paused.h"
#include "../CharacterStates/Normal.h"
#include "../CharacterStates/Jumping.h"
#include "../CharacterStates/Crouched.h"
#include "../CharacterStates/Running.h"
#include "../CharacterStates/Dying.h"
#include "../config/Constants.h"

class CharacterState;
class Enemy;
class PlatformNormal;

class Player : public GameObject {
public:
    Player(SDL_Rect *camera, std::string username, std::string textureID);
    ~Player() override;
    void init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, int framesAmount);
    void jump(int yMove);
    void run(int direction);

    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;

    bool isJumping();

    bool finishJump();

    void restartPos(int x, int y);

    void changeState(CharacterState* newState);

    void move(std::vector<int> vector);
    void move() override;
    std::string getUsername();

    void setUsername(std::string username);

    int getFrameAmount() override;

    void setPosition(int x, int y) override;

    void setDirection(bool direction) override;

    void setState(std::string state) override;
    std::string getState() override;

    bool getDirection() override;

    void collideWith(GameObject *go) override;

    //Collisions
    void collideWith(Enemy* enemy) override;
    void collideWith(Coin* coin) override;
    void collideWith(PlatformNormal* nBlock) override;
    void changeLevel();
    void addPoints(int newPoints);
    void die() override;
    int getWidth() override;

    int getLives() const;
    int loseLife();
    bool itsAlive();
    void testMode();
    bool getTestModeState();
    void startToJump();
    void setJumpConfig(bool restart);


    void restartPos();

    void finishMovement();

private:
    //Image related
    std::string username;
    static const int pWidth = 600;
    static const int pHeight = 600;
    static const int playerVelocity = 4;
    bool xDirection; //True = +x False = -x
    CharacterState* characterState;
    bool jumping;
    bool canJump() const;
    int initialJumpingPosition;
    int maxYPosition;
    SDL_Rect *cam;
    int ticks;
    bool leftOrRightPressed;

    void completeMovement(const Uint8 *keyStates);

    //Health related attributes
    bool isPlayerBig;
    int lives = 3;

    //Score related attributes
    int level = 1;
    int scorePosition;
    int actualScore;
    std::map<int, int> levelPoints;
    int floor;

    bool testModeState = false;

    void dropPlayer();

    int firstX;
    int firstY;
};


#endif //TPTALLER1_MARIO_H
