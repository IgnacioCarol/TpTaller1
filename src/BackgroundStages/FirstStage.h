//
// Created by nacho on 25/10/20.
//

#ifndef TPTALLER1_FIRSTSTAGE_H
#define TPTALLER1_FIRSTSTAGE_H


#include "BackgroundStage.h"

class FirstStage: public BackgroundStage {

    FirstStage() : BackgroundStage(nullptr, nullptr) {}

    BackgroundStage *nextStage();

public:
    FirstStage(TextureManager *pManager, SDL_Renderer *pRenderer);

    int getWidth() const override;

    bool setBackground() override;

};


#endif //TPTALLER1_FIRSTSTAGE_H