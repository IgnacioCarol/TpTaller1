//
// Created by DANIELA CARRERO on 2020-11-02.
//

#ifndef TPTALLER1_THIRDSTAGE_H
#define TPTALLER1_THIRDSTAGE_H

#include "../TextureManager.h"
#include "BackgroundStage.h"


class ThirdStage : public BackgroundStage {
public:
    ThirdStage();
    Timer* getTimer() override;
    ThirdStage(TextureManager *pManager, SDL_Renderer *pRenderer);

    BackgroundStage *nextStage() override;
};


#endif //TPTALLER1_THIRDSTAGE_H
