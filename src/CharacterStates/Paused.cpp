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
