//
// Created by Daniel Bizari on 26/10/2020.
//

#ifndef TPTALLER1_PLATFORMNORMAL_H
#define TPTALLER1_PLATFORMNORMAL_H

#include "GameObject.h"

class PlatformNormal : public GameObject {
    PlatformNormal();
    void init(int x, int y, std::string textureID, int currentFrame) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    virtual ~PlatformNormal();
};


#endif //TPTALLER1_PLATFORMNORMAL_H
