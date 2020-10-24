//
// Created by lisandro on 23/10/20.
//

#ifndef TPTALLER1_MARIO_H
#define TPTALLER1_MARIO_H


#include <string>
#include "TextureManager.h"

class Mario {
public:
    //Podria tener un estado, asi el draw se delega a ese estado
    Mario();
    void jump();
    void run(int direction);

    int getXPosition(){return xPosition;}

    void draw(SDL_Renderer* renderer){ //state = running, jumping normal en este caso es dino
        SDL_RendererFlip flip = (xDirection) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        int xFramePos = (jumping) ? 4 * 682 : currentFrame * 682; //TODO despues ver como no harcodear el size de la imagen
        textureManager->drawFrame("dino", xPosition, yPosition, 682, 474, xFramePos, 0, renderer, flip);
    }

    void setMarioState(std::string maritoState){ maritoState = maritoState;}

    bool isJumping(){return jumping;}

    bool finishJump(){ return initialJumpingPosition == yPosition;}

private:
    bool xDirection; //Despues hay que guiarse por otra cosa, bien hardcodeado. True = +x False = -x
    int xPosition;
    int yPosition;
    std::string marioState; //Puede ser running, jumping o normal
    TextureManager* textureManager = TextureManager::Instance();
    bool jumping;
    int initialJumpingPosition;
    int currentFrame = 0;
    int frames[5];
};


#endif //TPTALLER1_MARIO_H
