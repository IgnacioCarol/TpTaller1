//
// Created by lisandro on 25/10/20.
//

#ifndef TPTALLER1_JUMPING_H
#define TPTALLER1_JUMPING_H


#include "CharacterState.h"

class Jumping: public CharacterState {
public:
    explicit Jumping(bool falling = false);
    void move(const Uint8* currentKeyStates, Player* player) override;
    void changeState(const Uint8* currentKeyStates, Player* player) override;

private:
    static const int JUMPING_FRAME = 4;
};

#endif //TPTALLER1_JUMPING_H
