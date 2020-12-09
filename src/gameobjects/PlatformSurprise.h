#ifndef TPTALLER1_PLATFORMSURPRISE_H
#define TPTALLER1_PLATFORMSURPRISE_H
#include "GameObject.h"

class PlatformSurprise : public GameObject {
public:
    ~PlatformSurprise() override = default;
    void init(int x, int y, std::string textureID) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    void move() override {};
    int getHeight() override;

private:
    //Image related definitions
    static const int SURPRISE_BLOCK_WIDTH = 256;
    static const int SURPRISE_BLOCK_HEIGHT = 256;
    //Animation related definitions
    static const int SURPRISE_BLOCK_DELAY = 200;
    static const int SURPRISE_BLOCK_FRAMES = 4;
    size_t delayCounter;

};


#endif //TPTALLER1_PLATFORMSURPRISE_H
