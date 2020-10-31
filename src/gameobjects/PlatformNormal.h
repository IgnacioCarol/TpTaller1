#ifndef TPTALLER1_PLATFORMNORMAL_H
#define TPTALLER1_PLATFORMNORMAL_H

#include "GameObject.h"

//Image related definitions
#define NBWidth 256
#define NBHeight 256
#define nBlockID "normalBlock"

class PlatformNormal : public GameObject {
    public:
        PlatformNormal();
        void init(int x, int y, std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        virtual ~PlatformNormal();
};


#endif //TPTALLER1_PLATFORMNORMAL_H
