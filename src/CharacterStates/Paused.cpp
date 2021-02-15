//
// Created by lisandro on 28/11/20.
//

#include "Paused.h"
#include "../gameobjects/Player.h"
#include "../Server/GameServer.h"


Paused::Paused(bool disconnected) {
    stateType = disconnected ? "PAUSED" : "FINISH";
    this -> disconnected = disconnected;
    currentFrame = PAUSED_FRAME;
}

void Paused::move(const Uint8 *currentKeyStates, Player *player) {
    int camXPos = GameServer::Instance()->getCamera()->getXpos();
    if (player -> getXPosition() <= camXPos){
        player -> setDirection(true);
        player -> setPosition(camXPos + 3, player->getYPosition());
    }
}

void Paused::changeState(const Uint8 *currentKeyStates, Player *player) {
    CharacterState::changeState(currentKeyStates, player);
}

void Paused::draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth, SDL_Renderer *renderer,
                  SDL_RendererFlip flip) {
    if (stateType == "PAUSED") {
       //ID = this->isPlayerBig ? "paused-big" : "paused";
    }

    CharacterState::draw(ID, xPosition, yPosition, imageWidth, imageHeigth, renderer, flip);
}
