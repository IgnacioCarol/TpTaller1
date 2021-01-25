//
// Created by lisandro on 13/1/21.
//

#include "Dying.h"

Dying::Dying(){
    stateType = "DYING";
    currentFrame = DYING_FRAME;
}

void Dying::move(const Uint8 *currentKeyStates, Player *player) {
    if (countAux > ITER_TIMES * 2){
        if ((player->getYPosition() >= initialY - EXTRA_HEIGHT) && !descending){
            player->setPosition(player->getXPosition(), player->getYPosition() - GRAVITY);
        }
        else{
            descending = true;
            player->setPosition(player->getXPosition(), player->getYPosition() + GRAVITY);
        }
    }
    else{
        initialY = player->getYPosition();
        countAux++;
    }
}
