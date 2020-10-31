#ifndef TPTALLER1_PLATFORMNORMAL_H
#define TPTALLER1_PLATFORMNORMAL_H

#include "GameObject.h"

//Image related definitions
#define NBWidth 88
#define NBHeight 86
#define nBlockID "normalBlock"

class PlatformNormal : public GameObject {
    public:
        PlatformNormal();
        void init(int x, int y, std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        virtual ~PlatformNormal();
};


#endif //TPTALLER1_PLATFORMNORMAL_H
