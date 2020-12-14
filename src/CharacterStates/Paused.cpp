//
// Created by lisandro on 28/11/20.
//

#include "Paused.h"
#include "../gameobjects/Player.h"
#include "../Server/GameServer.h"


Paused::Paused(int currentFrame, int frameAmount, bool disconnected) : CharacterState(currentFrame, frameAmount) {
    stateType = disconnected ? "PAUSED" : "FINISH";
    this -> disconnected = disconnected;
}

void Paused::move(const Uint8 *currentKeyStates, Player *player) {
    int camXPos = GameServer::Instance()->getCamera()->getXpos();
    if (player -> getXPosition() <= camXPos){
        player -> setDirection(true);
        player -> setPosition(camXPos + 2, player->getYPosition());
    }
}

void Paused::changeState(const Uint8 *currentKeyStates, Player *player) {
    CharacterState::changeState(currentKeyStates, player);
}

void Paused::draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth, SDL_Renderer *renderer,
                  SDL_RendererFlip flip) {
    ID = (stateType == "PAUSED") ? "paused" : ID;
    CharacterState::draw(ID, xPosition, yPosition, imageWidth, imageHeigth, renderer, flip);
}
