//
// Created by Daniel Bizari on 31/10/2020.
//
#include "../gameobjects/Player.h"
#include "Running.h"
#include "Jumping.h"
#include "Normal.h"
#include "Crouched.h"

Crouched::Crouched(int currentFrame, int frameAmount) : CharacterState(currentFrame, frameAmount) {

}

void Crouched::changeState(const Uint8 *currentKeyStates, Player *player) {
    if (currentKeyStates[SDL_SCANCODE_UP]){
        player -> changeState(new Jumping(framesAmount -1, framesAmount));
    }

    else if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_LEFT]){
        player -> changeState(new Running(currentFrame, framesAmount));
    }

    else if (!currentKeyStates[SDL_SCANCODE_DOWN]){
        CharacterState * tmp = player->getCurrentState();
        player -> changeState(new Normal(0, framesAmount));
        delete tmp; //ToDo recordar eliminar los otros estados
    }
}

void Crouched::move(const Uint8* currentKeyStates, Player* player) {
    // In this state the player cannot move
}
