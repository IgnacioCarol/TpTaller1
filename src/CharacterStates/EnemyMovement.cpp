//
// Created by lisandro on 27/10/20.
//

#include "EnemyMovement.h"

EnemyMovement::EnemyMovement(int currentFrame, int framesAmount) : CharacterState(currentFrame, framesAmount) {

}

void EnemyMovement::move(Enemy *enemy) {
    enemy->walk();
}

void EnemyMovement::draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth,
                         SDL_Renderer *renderer, SDL_RendererFlip flip) {
    if (contAux == ITER_TIMES) {
        currentFrame = (currentFrame + 1) % (framesAmount - 1);
        contAux = 0;
    }
    else contAux ++;

    TextureManager::Instance()->drawFrame(ID, xPosition, yPosition, imageWidth, imageHeigth,
                                          imageWidth * currentFrame, renderer, flip);
}




