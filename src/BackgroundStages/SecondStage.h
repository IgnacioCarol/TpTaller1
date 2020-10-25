//
// Created by nacho on 25/10/20.
//

#ifndef TPTALLER1_SECONDSTAGE_H
#define TPTALLER1_SECONDSTAGE_H


#include "src/TextureManager.h"
#include "BackgroundStage.h"

class SecondStage: public BackgroundStage {
    SecondStage() : BackgroundStage(nullptr, nullptr) {}

public:
    SecondStage(TextureManager *pManager, SDL_Renderer *pRenderer);

    int getWidth() const override;

    bool setBackground() override;

    BackgroundStage *nextStage() override;
};


#endif //TPTALLER1_SECONDSTAGE_H
