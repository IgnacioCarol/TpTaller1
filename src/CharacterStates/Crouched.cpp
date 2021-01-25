//
// Created by Daniel Bizari on 31/10/2020.
//
#include "../gameobjects/Player.h"
#include "Running.h"
#include "Jumping.h"
#include "Normal.h"
#include "Crouched.h"

Crouched::Crouched(){
    stateType = "CROUCHED";
    currentFrame = CROUCHED_FRAME;
}

void Crouched::changeState(const Uint8 *currentKeyStates, Player *player) {
    if (!currentKeyStates[SDL_SCANCODE_DOWN]) {
        player->changeState(new Normal());
    }
}

void Crouched::move(const Uint8* currentKeyStates, Player* player) {
    // In this state the player cannot move
}
