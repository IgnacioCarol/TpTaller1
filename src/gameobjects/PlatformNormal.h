#ifndef TPTALLER1_PLATFORMNORMAL_H
#define TPTALLER1_PLATFORMNORMAL_H

#include "GameObject.h"

//Image related definitions
#define NBWidth 256
#define NBHeight 256

class PlatformNormal : public GameObject {
    public:
        PlatformNormal();
        ~PlatformNormal() override = default;
        void init(int x, int y, std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        void move() override {};
};


#endif //TPTALLER1_PLATFORMNORMAL_H
