//
// Created by lisandro on 13/1/21.
//

#ifndef TPTALLER1_DYING_H
#define TPTALLER1_DYING_H


#include "CharacterState.h"
#include "../gameobjects/Player.h"

class Dying: public CharacterState{
public:
    explicit Dying();
    void move(const Uint8 *currentKeyStates, Player *player) override;

private:
    int initialY = 0;
    int countAux = 0;
    static const int GRAVITY = 3;
    static const int EXTRA_HEIGHT = 80; //To do the effect that marios goes up and then goes down when he dies
    bool descending = false;

};


#endif //TPTALLER1_DYING_H
