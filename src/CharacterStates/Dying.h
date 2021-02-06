//
// Created by lisandro on 13/1/21.
//

#ifndef TPTALLER1_DYING_H
#define TPTALLER1_DYING_H


#include "CharacterState.h"
#include "../gameobjects/Player.h"
#include "../gameobjects/Enemy.h"

class Dying: public CharacterState{
public:
    explicit Dying(int currentFrame = 0, bool falling = false);
    void move(Enemy* enemy) override;
    void move(const Uint8 *currentKeyStates, Player *player) override;

private:
    static const int DYING_FRAME = 6;
    int initialY = 0;
    int countAux = 0;
    static const int GRAVITY = 3;
    static const int EXTRA_HEIGHT = 80; //To do the effect that marios goes up and then goes down when he dies
    bool descending = false;

    static const int DIE_COUNTER = 4;
    int counter = 0;

};


#endif //TPTALLER1_DYING_H
