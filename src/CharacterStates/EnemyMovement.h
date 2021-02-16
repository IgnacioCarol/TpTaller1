//
// Created by lisandro on 27/10/20.
//

#ifndef TPTALLER1_ENEMYMOVEMENT_H
#define TPTALLER1_ENEMYMOVEMENT_H

#include "../gameobjects/Enemy.h"
#include "CharacterState.h"

class EnemyMovement: public CharacterState {
public:
    explicit EnemyMovement(int currentFrame = 0, int framesAmount = 1);
    void move(Enemy* enemy) override;
    void draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth,
              SDL_Renderer* renderer, SDL_RendererFlip flip, int divider) override;
};


#endif //TPTALLER1_ENEMYMOVEMENT_H
