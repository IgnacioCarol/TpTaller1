#include "../TextureManager.h"
#include "CharacterState.h"

CharacterState::CharacterState(int currentFrame, int framesAmount) {
    this -> currentFrame = currentFrame;
    this -> framesAmount = framesAmount;
}

CharacterState::CharacterState() {
}

void CharacterState::draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth,
                          SDL_Renderer *renderer, SDL_RendererFlip flip, int divider) {

    TextureManager::Instance()->drawFrame(ID, xPosition, yPosition, imageWidth, imageHeigth,
                                          imageWidth * currentFrame, renderer, flip, divider);
}

void CharacterState::move(const Uint8 *currentKeyStates, Player *player) {

}

void CharacterState::changeState(const Uint8 *currentKeyStates, Player *player) {

}

void CharacterState::move(Enemy *enemy) {

}

int CharacterState::getFramesAmount() {
    return framesAmount;
}

std::string CharacterState::getStateType() {
    return stateType;
}

