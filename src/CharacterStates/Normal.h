//
// Created by lisandro on 25/10/20.
//

#ifndef TPTALLER1_NORMAL_H
#define TPTALLER1_NORMAL_H


#include "CharacterState.h"
class Normal: public CharacterState{
public:
    explicit Normal(bool isPlayerBig);
    void move(const Uint8* currentKeyStates, Player* player) override;
    void changeState(const Uint8* currentKeyStates, Player* player) override;

private:
    static const int NORMAL_FRAME = 0;
};


#endif //TPTALLER1_NORMAL_H
