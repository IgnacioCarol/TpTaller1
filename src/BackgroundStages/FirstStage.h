//
// Created by nacho on 25/10/20.
//

#ifndef TPTALLER1_FIRSTSTAGE_H
#define TPTALLER1_FIRSTSTAGE_H

#include "BackgroundStage.h"
#include "SecondStage.h"
#include "../Game.h"

class FirstStage: public BackgroundStage {
public:
    FirstStage();

    FirstStage(TextureManager *pManager, SDL_Renderer *pRenderer);

    BackgroundStage *nextStage() override;
};


#endif //TPTALLER1_FIRSTSTAGE_H
