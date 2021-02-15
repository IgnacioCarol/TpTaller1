#ifndef TPTALLER1_MARIO_H
#define TPTALLER1_MARIO_H


#include <string>
#include "../logger/logger.h"
#include "../TextureManager.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Coin.h"
#include "PlatformNormal.h"
#include "Hole.h"
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
#define INMUNITY_TIME 50

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
    bool getPlayerBig();
    void collideWith(GameObject *go) override;

    bool isActive();

    //Collisions
    void collideWith(Enemy* enemy) override;
    void collideWith(Coin* coin) override;
    void collideWith(PlatformNormal* nBlock) override;
    void collideWith(Hole* hole) override;
    void saveLevelPoints(int currentLevel);

    //Lives
    int getLives() const;
    void setLives(int totalLives);
    void loseLife();
    bool isAlive();

    //Points
    void setPoints(int points);
    std::map<int,int> getPointsByLevel();
    void setPointsByLevel(std::map<int,int> points);
    int getTotalPoints();
    void addPoints(int newPoints);

    void die() override;
    void dieFalling() override;
    void fall();
    int getWidth() override;

    void setPlayerBig(bool playerBig);
    void testMode();
    bool getTestModeState();
    void setTestMode(bool testModeState);
    void startToJump();
    void setJumpConfig();


    void restartPos();

    bool isInmune();
    void tryUndoInmunity();

    bool operator<(const Player& p) const;

    void finishMovement();

    int getFloorPosition() override;

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
    bool isPlayerBig = false;
    int inmune;
    int lives = 3;

    //Score related attributes
    int level = 1;
    int floor;

    bool testModeState = false;
    int totalPoints = 0;
    int partialPoints = 0;
    std::map<int, int> levelPoints;

    void activateInmunity();
    void dropPlayer();

    int firstX;
    int firstY;
    int ticksAfterRespawning;
    int divider; //For drawing bir or normal Mario. divider = 5 --> Small Mario, divider = 4 --> Big Mario
};


#endif //TPTALLER1_MARIO_H
