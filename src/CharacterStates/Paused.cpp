//
// Created by lisandro on 28/11/20.
//

#include "Paused.h"
#include "../gameobjects/Player.h"
#include "../Server/GameServer.h"


Paused::Paused(int currentFrame, int frameAmount) : CharacterState(currentFrame, frameAmount) {
    stateType = "PAUSED";
}

void Paused::move(const Uint8 *currentKeyStates, Player *player) {

    if (player -> getXPosition() <= GameServer::Instance()->getCamera()->getXpos()){
        player -> run (1);
    }
}

void Paused::changeState(const Uint8 *currentKeyStates, Player *player) {
    CharacterState::changeState(currentKeyStates, player);
}

void Paused::draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth, SDL_Renderer *renderer,
                  SDL_RendererFlip flip) {
    CharacterState::draw("paused", xPosition, yPosition, imageWidth, imageHeigth, renderer, flip);
}
