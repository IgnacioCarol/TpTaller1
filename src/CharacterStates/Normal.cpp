//
// Created by lisandro on 25/10/20.
//

#include "../gameobjects/Player.h"
#include "Normal.h"
#include "Running.h"
#include "Jumping.h"
#include "Crouched.h"

Normal::Normal(bool isPlayerBig){
    stateType = "NORMAL";
    currentFrame = NORMAL_FRAME;
    this->isPlayerBig = isPlayerBig;
}

void Normal::move(const Uint8 *currentKeyStates, Player* player) {} //Nothing to do

void Normal::changeState(const Uint8 *currentKeyStates, Player* player) {
    if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_LEFT]){
        player -> changeState(new Running(this->isPlayerBig));
    }

    else if (currentKeyStates[SDL_SCANCODE_UP]){
        player -> changeState(new Jumping(this->isPlayerBig));
    }

    else if (currentKeyStates[SDL_SCANCODE_DOWN]){
        player -> changeState(new Crouched(this->isPlayerBig));
    }
}
