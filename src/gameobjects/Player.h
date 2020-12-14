#ifndef TPTALLER1_MARIO_H
#define TPTALLER1_MARIO_H


#include <string>
#include "../logger/logger.h"
#include "../TextureManager.h"
#include "GameObject.h"

#define imgPlayer "Sprites/Players/mario.png"
#define defaultPlayer "Sprites/Default/defaultPlayer.png"

class CharacterState;

class Player : public GameObject {
public:
    Player(SDL_Rect *camera, std::string username, std::string textureID);
    ~Player() override;
    void init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, int framesAmount);
    void jump(int yMove);
    void run(int direction);

    int getXPosition(){return xPosition;}

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

private:
    //Image related
    std::string username;
    static const int pWidth = 600;
    static const int pHeight = 600;
    static const int playerVelocity = 2;
    bool xDirection; //True = +x False = -x
    CharacterState* characterState;
    bool jumping;
    bool canJump() const;
    int initialJumpingPosition;
    int maxYPosition;
    SDL_Rect *cam;
    int ticks;
    bool leftOrRightPressed;
};


#endif //TPTALLER1_MARIO_H
