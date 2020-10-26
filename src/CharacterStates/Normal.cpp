//
// Created by lisandro on 25/10/20.
//

#include "../Player.h"
#include "Normal.h"
#include "Running.h"
#include "Jumping.h"


Normal::Normal(int currentFrame, int frameAmount) : CharacterState(currentFrame, frameAmount) {
}

void Normal::move(const Uint8 *currentKeyStates, Player* player) {} //Nothing to do

void Normal::changeState(const Uint8 *currentKeyStates, Player* player) {
    if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_LEFT]){
        player -> changeState(new Running(currentFrame, framesAmount));
    }

    else if (currentKeyStates[SDL_SCANCODE_UP]){
        player -> changeState(new Jumping(framesAmount - 1, framesAmount));
    }
}
