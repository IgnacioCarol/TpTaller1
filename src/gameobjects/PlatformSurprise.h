#ifndef TPTALLER1_PLATFORMSURPRISE_H
#define TPTALLER1_PLATFORMSURPRISE_H

//Image related definitions
#define SBWidth 256
#define SBHeight 256
#define defaultBlock "Sprites/Default/defaultBlock.png"
#define sBlockID "surpriseBlockSprites" //Platform Surprise

//Animation related definitions
#define sBlockDelay 200
#define sBlockFrames 4

#include "GameObject.h"

class PlatformSurprise : public GameObject {
    public:
        PlatformSurprise();
        ~PlatformSurprise() override = default;
        void init(int x, int y, std::string fileName, std::string defaultImg, std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        void move() override {};
        std::string getFilePath() override;
        std::string getID() override;
        std::string getDefault() override;

private:
    size_t delayCounter;
};


#endif //TPTALLER1_PLATFORMSURPRISE_H
