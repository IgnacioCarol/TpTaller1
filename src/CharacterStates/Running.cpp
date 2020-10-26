//
// Created by lisandro on 25/10/20.
//

#include "../gameobjects/Player.h"
#include "Running.h"
#include "Jumping.h"
#include "Normal.h"

Running::Running(int currentFrame, int frameAmount) : CharacterState(currentFrame, frameAmount) {

}

void Running::move(const Uint8 *currentKeyStates, Player* player) {
    player -> run(currentKeyStates[ SDL_SCANCODE_RIGHT ] - currentKeyStates[ SDL_SCANCODE_LEFT ]);
}

void Running::changeState(const Uint8 *currentKeyStates, Player* player) {
    if (currentKeyStates[SDL_SCANCODE_UP]){
        player -> changeState(new Jumping(framesAmount -1, framesAmount));
    }

    else if (!(currentKeyStates[ SDL_SCANCODE_RIGHT ] || currentKeyStates[ SDL_SCANCODE_LEFT ])){
        player -> changeState(new Normal(0, framesAmount));
    }
}

void
Running::draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth, SDL_Renderer *renderer,
              SDL_RendererFlip flip) {
    currentFrame = (currentFrame + 1) % (framesAmount - 1);
    TextureManager::Instance()->drawFrame(ID, xPosition, yPosition, imageWidth, imageHeigth,
                                          imageWidth * currentFrame, renderer, flip);
}
