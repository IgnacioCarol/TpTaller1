//
// Created by lisandro on 25/10/20.
//

#ifndef TPTALLER1_NORMAL_H
#define TPTALLER1_NORMAL_H


#include "CharacterState.h"
class Normal: public CharacterState{
public:
    explicit Normal(int currentFrame = 0, int frameAmount = 1);
    void move(const Uint8* currentKeyStates, Player* player) override;
    void changeState(const Uint8* currentKeyStates, Player* player) override;

};


#endif //TPTALLER1_NORMAL_H
