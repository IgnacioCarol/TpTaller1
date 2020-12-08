//
// Created by lisandro on 28/11/20.
//

#include "Paused.h"
#include "../gameobjects/Player.h"
#include "../Game.h"


Paused::Paused(int currentFrame, int frameAmount) : CharacterState(currentFrame, frameAmount) {

}

void Paused::move(const Uint8 *currentKeyStates, Player *player) {

    if (player -> getXPosition() <= Game::Instance()->getCamera()->x){
        player -> run (1);
    }
}

void Paused::changeState(const Uint8 *currentKeyStates, Player *player) {
    CharacterState::changeState(currentKeyStates, player);
}

void Paused::draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth, SDL_Renderer *renderer,
                  SDL_RendererFlip flip) {
    CharacterState::draw(ID, xPosition, yPosition, imageWidth, imageHeigth, renderer, flip);
    //+ "Paused" TODO cuando tengamos las imagenes griseadas agregar esto al parametro ID
}
