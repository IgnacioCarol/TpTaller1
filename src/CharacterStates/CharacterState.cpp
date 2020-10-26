//
// Created by lisandro on 25/10/20.
//

#include "../TextureManager.h"
#include "CharacterState.h"

void CharacterState::draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth,
                          SDL_Renderer *renderer, SDL_RendererFlip flip) {

    TextureManager::Instance()->drawFrame(ID, xPosition, yPosition, imageWidth, imageHeigth,
                                          imageWidth * currentFrame, renderer, flip);
}

CharacterState::CharacterState(int currentFrame, int framesAmount) {
    this -> currentFrame = currentFrame;
    this -> framesAmount = framesAmount;

}

void CharacterState::move(const Uint8 *currentKeyStates, Player *player) {

}

void CharacterState::changeState(const Uint8 *currentKeyStates, Player *player) {

}
