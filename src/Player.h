#ifndef TPTALLER1_MARIO_H
#define TPTALLER1_MARIO_H


#include <string>
#include <src/logger/logger.h>
#include "TextureManager.h"
#include "GameObject.h"

//Image related
#define pWidth 682
#define pHeight 474
#define pFrameMultiplier 4

class Player : public GameObject {
public:
    //Podria tener un estado, asi el draw se delega a ese estado
    Player();
    void init(size_t x, size_t y, std::string textureID, int currentFrame, SDL_Rect *camera);
    void jump(int yMove);
    void run(int direction);

    int getXPosition(){return xPosition;}

    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) { //state = running, jumping normal en este caso es dino
        SDL_RendererFlip flip = (xDirection) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        int xFramePos = (jumping) ? pFrameMultiplier * pWidth : _currentFrame * pWidth;
        textureManager->drawFrame("dino", xPosition - cameraX, yPosition - cameraY, pWidth, pHeight, xFramePos, 0, renderer, flip);
    }

    void setPlayerState(std::string state){ playerState = state;}

    bool isJumping(){return jumping;}

    bool finishJump(){ return initialJumpingPosition == yPosition;}

    void restartPos(int i, int i1);

private:
    bool xDirection; //Despues hay que guiarse por otra cosa, bien hardcodeado. True = +x False = -x
    std::string playerState; //Puede ser running, jumping o normal

    bool jumping;
    bool canJump() const;
    int initialJumpingPosition;
    int maxYPosition;
    int frames[5];
    SDL_Rect *cam;
    Logger* logger = Logger::getInstance();
};


#endif //TPTALLER1_MARIO_H
