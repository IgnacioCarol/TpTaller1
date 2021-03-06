//
// Created by lisandro on 25/10/20.
//

#include "../gameobjects/Player.h"
#include "Running.h"
#include "Jumping.h"
#include "Normal.h"
#include "Crouched.h"

Running::Running() {
    stateType = "RUNNING";
    currentFrame = RUNNING_FRAME;
}

void Running::move(const Uint8 *currentKeyStates, Player* player) {
    player -> run(currentKeyStates[ SDL_SCANCODE_RIGHT ] - currentKeyStates[ SDL_SCANCODE_LEFT ]);
}

void Running::changeState(const Uint8 *currentKeyStates, Player* player) {
    if (currentKeyStates[SDL_SCANCODE_UP]){
        player -> startToJump();
        player -> changeState(new Jumping());
    }

    else if (currentKeyStates[SDL_SCANCODE_DOWN]){
        player -> changeState(new Crouched());
    }

    else if (!(currentKeyStates[ SDL_SCANCODE_RIGHT ] || currentKeyStates[ SDL_SCANCODE_LEFT ])){
        player -> changeState(new Normal());
    }
}

void Running::draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth,
                   SDL_Renderer *renderer,SDL_RendererFlip flip, int divider) {
    if (contAux == ITER_TIMES) {
        currentFrame = (currentFrame % (RUNNING_FRAME + 2)) + 1;
        contAux = 0;
    }
    else contAux++;

    TextureManager::Instance()->drawFrame(ID, xPosition, yPosition, imageWidth, imageHeigth,
                                          imageWidth * currentFrame, renderer, flip, divider);
}
