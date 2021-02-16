//
// Created by lisandro on 28/11/20.
//

#ifndef TPTALLER1_PAUSED_H
#define TPTALLER1_PAUSED_H
#include "CharacterState.h"

class Paused: public CharacterState {
public:
    explicit Paused(bool disconnected = true);
    void move(const Uint8* currentKeyStates, Player* player) override;
    void changeState(const Uint8* currentKeyStates, Player* player) override;
private:
    bool disconnected;
    static const int PAUSED_FRAME = 0;
};


#endif //TPTALLER1_PAUSED_H
