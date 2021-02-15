//
// Created by lisandro on 25/10/20.
//

#ifndef TPTALLER1_RUNNING_H
#define TPTALLER1_RUNNING_H


#include "CharacterState.h"

class Running: public CharacterState {
public:
    explicit Running(bool isPlayerBig);
    void move(const Uint8* currentKeyStates, Player* player) override;
    void changeState(const Uint8* currentKeyStates, Player* player) override;
    void draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth,
              SDL_Renderer* renderer, SDL_RendererFlip flip) override;
private:
    static const int RUNNING_FRAME = 1;
};


#endif //TPTALLER1_RUNNING_H
