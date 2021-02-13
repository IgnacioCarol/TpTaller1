//
// Created by nacho on 25/10/20.
//

#ifndef TPTALLER1_SECONDSTAGE_H
#define TPTALLER1_SECONDSTAGE_H


#include "../TextureManager.h"
#include "BackgroundStage.h"
#include "ThirdStage.h"

class SecondStage: public BackgroundStage {
public:
    SecondStage();

    SecondStage(TextureManager *pManager, SDL_Renderer *pRenderer);

    BackgroundStage *nextStage() override;
};


#endif //TPTALLER1_SECONDSTAGE_H
