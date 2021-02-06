//
// Created by lisandro on 13/1/21.
//

#include "Dying.h"

#include <utility>

Dying::Dying(int currentFrame, bool falling){
    stateType = (!falling) ? "DYING" : "DYING_FALLING";
    this -> currentFrame = (currentFrame) ? currentFrame : DYING_FRAME;
}

void Dying::move(const Uint8 *currentKeyStates, Player *player) {
    if (stateType == "DYING"){
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
}

void Dying::move(Enemy *enemy) {
    if (counter++ == DIE_COUNTER){
        enemy->die();
    }
}
