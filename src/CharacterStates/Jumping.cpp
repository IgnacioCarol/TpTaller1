//
// Created by lisandro on 25/10/20.
//

#include "../gameobjects/Player.h"
#include "Jumping.h"
#include "Normal.h"

Jumping::Jumping(bool falling){
    stateType = (falling) ? "FALLING" : "JUMPING";
    currentFrame = JUMPING_FRAME;
}

void Jumping::move(const Uint8 *currentKeyStates, Player* player) {
    player -> run(currentKeyStates[ SDL_SCANCODE_RIGHT ] - currentKeyStates[ SDL_SCANCODE_LEFT ]);
    player -> jump(currentKeyStates[ SDL_SCANCODE_UP]);
}

void Jumping::changeState(const Uint8 *currentKeyStates, Player* player) {

    if (stateType == "FALLING"){
        player->dieFalling();
    }
    else{
        player -> changeState(new Normal());
    }
}
