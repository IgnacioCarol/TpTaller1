//
// Created by Daniel Bizari on 26/10/2020.
//

#ifndef TPTALLER1_PLATFORMSURPRISE_H
#define TPTALLER1_PLATFORMSURPRISE_H

#include "GameObject.h"

class PlatformSurprise : public GameObject {
    public:
        PlatformSurprise();
        void init(int x, int y, std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        virtual ~PlatformSurprise();
};


#endif //TPTALLER1_PLATFORMSURPRISE_H
