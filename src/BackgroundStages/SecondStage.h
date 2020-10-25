//
// Created by nacho on 25/10/20.
//

#ifndef TPTALLER1_SECONDSTAGE_H
#define TPTALLER1_SECONDSTAGE_H


#include "TextureManager.h"
#include "BackgroundStage.h"

class SecondStage: public BackgroundStage {

public:
    SecondStage(TextureManager *pManager, SDL_Renderer *pRenderer);
};


#endif //TPTALLER1_SECONDSTAGE_H
