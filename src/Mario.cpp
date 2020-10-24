//
// Created by lisandro on 23/10/20.
//

#include <cstdio>
#include "Mario.h"


Mario::Mario() {
    xPosition = 0;
    yPosition = 403; //Por defecto lo creo ahi
    marioState = "dino"; //dino jumpDino, runDino, asi accedo a la imagen en el map del TextureManager TODO cambiar por clase state
    xDirection = true;
    jumping = false;
    initialJumpingPosition = 403;
    maxYPosition = yPosition - 100;
    frames[0]= 0;
    frames[1]= 1;
    frames[2]= 2;
    frames[3]= 3;
    frames[4]= 4;
}

void Mario::run(int direction) {
    currentFrame = (frames[currentFrame] + 1) % ((sizeof(frames) / sizeof(frames[0])) - 1);
    xDirection = direction ? direction > 0 : xDirection;
    //direction puede ser +x o -x  hacemos que valga +- 1 por ahora
    //el y se mantiene igual
    xPosition += direction;
    /*if (!jumping){
        marioState = (marioState == "dino") ? "runDino" : "dino";
    }
    if (jumping || (!jumping && yPosition != initialJumpingPosition)){
        this->jump(); //para que avance mientras salto
    }*/
    marioState = (marioState != "jumpDino" && direction) ? "runDino" : marioState;
}

void Mario::jump(int yMovement) {
    bool isNotStartingPos = yPosition < initialJumpingPosition;
    if ((jumping = canJump())) {
        yPosition = yPosition + (!isNotStartingPos || yMovement ? - yMovement : + 1);
    } else if (isNotStartingPos) {
        yPosition += 1;
    }
    marioState = (isNotStartingPos) ? "jumpDino" : "dino";
}

bool Mario::canJump() const {
    return ((jumping && yPosition > maxYPosition) || (!jumping && yPosition == initialJumpingPosition));
}
