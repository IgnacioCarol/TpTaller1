#ifndef TPTALLER1_PLATFORMSURPRISE_H
#define TPTALLER1_PLATFORMSURPRISE_H

//Image related definitions
#define SBWidth 256
#define SBHeight 256

//Animation related definitions
#define sBlockDelay 200
#define sBlockFrames 4

#include "GameObject.h"

class PlatformSurprise : public GameObject {
    public:
        PlatformSurprise();
        void init(int x, int y, std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        void move() override {};
        virtual ~PlatformSurprise();

private:
    size_t delayCounter;
};


#endif //TPTALLER1_PLATFORMSURPRISE_H
