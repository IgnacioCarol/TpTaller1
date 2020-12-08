#ifndef TPTALLER1_MARIO_H
#define TPTALLER1_MARIO_H


#include <string>
#include "../logger/logger.h"
#include "../TextureManager.h"
#include "GameObject.h"

#define playerID "mario" //Principal Player
#define imgPlayer "Sprites/Players/mario.png"
#define defaultPlayer "Sprites/Default/defaultPlayer.png"

class CharacterState;

class Player : public GameObject {
public:
    Player(SDL_Rect *camera, std::string username);
    ~Player();
    void init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, int framesAmount);
    void jump(int yMove);
    void run(int direction);

    int getXPosition(){return xPosition;}

    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;

    bool isJumping();

    bool finishJump();

    void restartPos(int i, int i1);

    void changeState(CharacterState* newState);

    void move() override;

    std::string getUsername();

    void setUsername(std::string username);

    int getFrameAmount() override;

private:
    //Image related
    std::string username;
    static const int pWidth = 600;
    static const int pHeight = 600;
    static const int playerVelocity = 2;
    bool xDirection; //Despues hay que guiarse por otra cosa, bien hardcodeado. True = +x False = -x
    CharacterState* characterState;
    bool jumping;
    bool canJump() const;
    int initialJumpingPosition;
    int maxYPosition;
    SDL_Rect *cam;
};


#endif //TPTALLER1_MARIO_H
