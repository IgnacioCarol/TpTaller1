//
// Created by lisandro on 25/10/20.
//

#include "../gameobjects/Player.h"
#include "Normal.h"
#include "Running.h"
#include "Jumping.h"
#include "Crouched.h"

Normal::Normal() : CharacterState() {
    stateType = "NORMAL";
    currentFrame = NORMAL_FRAME;
}

void Normal::move(const Uint8 *currentKeyStates, Player* player) {} //Nothing to do

void Normal::changeState(const Uint8 *currentKeyStates, Player* player) {
    if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_LEFT]){
        player -> changeState(new Running());
    }

    else if (currentKeyStates[SDL_SCANCODE_UP]){
        player -> changeState(new Jumping());
    }

    else if (currentKeyStates[SDL_SCANCODE_DOWN]){
        player -> changeState(new Crouched());
    }
}
