//
// Created by lisandro on 25/10/20.
//

#include "../gameobjects/Player.h"
#include "Running.h"
#include "Jumping.h"
#include "Normal.h"
#include "Crouched.h"

Running::Running(int currentFrame, int frameAmount) : CharacterState(currentFrame, frameAmount) {
    stateType = "RUNNING";
}

void Running::move(const Uint8 *currentKeyStates, Player* player) {
    player -> run(currentKeyStates[ SDL_SCANCODE_RIGHT ] - currentKeyStates[ SDL_SCANCODE_LEFT ]);
}

void Running::changeState(const Uint8 *currentKeyStates, Player* player) {
    if (currentKeyStates[SDL_SCANCODE_UP]){
        player -> changeState(new Jumping(framesAmount - RUNNING_FRAME, framesAmount));
    }

    else if (currentKeyStates[SDL_SCANCODE_DOWN]){
        player -> changeState(new Crouched(framesAmount - CROUCHED_FRAME, framesAmount)); //ToDo revisar currentFrame
    }

    else if (!(currentKeyStates[ SDL_SCANCODE_RIGHT ] || currentKeyStates[ SDL_SCANCODE_LEFT ])){
        player -> changeState(new Normal(0, framesAmount));
    }
}

void Running::draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth,
                   SDL_Renderer *renderer,SDL_RendererFlip flip) {
    if (contAux == ITER_TIMES) {
        currentFrame = (currentFrame + 1) % (framesAmount - RUNNING_FRAME);
        contAux = 0;
    }
    else contAux++;

    TextureManager::Instance()->drawFrame(ID, xPosition, yPosition, imageWidth, imageHeigth,
                                          imageWidth * currentFrame, renderer, flip);
}
