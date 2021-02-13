//
// Created by Daniel Bizari on 31/10/2020.
//

#ifndef TPTALLER1_CROUCHED_H
#define TPTALLER1_CROUCHED_H

#include "CharacterState.h"

class Crouched : public CharacterState{
    public:
        explicit Crouched(bool isPlayerBig);
        void move(const Uint8* currentKeyStates, Player* player) override;
        void changeState(const Uint8* currentKeyStates, Player* player) override;

private:
    static const int CROUCHED_FRAME = 5;
};

#endif //TPTALLER1_CROUCHED_H
