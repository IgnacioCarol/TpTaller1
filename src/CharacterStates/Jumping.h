//
// Created by lisandro on 25/10/20.
//

#ifndef TPTALLER1_JUMPING_H
#define TPTALLER1_JUMPING_H


#include "CharacterState.h"

class Jumping: public CharacterState {
public:
    explicit Jumping();
    void move(const Uint8* currentKeyStates, Player* player) override;
    void changeState(const Uint8* currentKeyStates, Player* player) override;

};

#endif //TPTALLER1_JUMPING_H
