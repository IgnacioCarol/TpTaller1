#ifndef TPTALLER1_COIN_H
#define TPTALLER1_COIN_H

#include "GameObject.h"
#define coinDelay 100

//Image related definitions
#define CWidth 161
#define CHeight 164

//Animation related definitions
#define coinsFrames 4

class Coin : public GameObject {
    public:
        Coin();
        ~Coin() override;
        void init(int x, int y, std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        void move() override {};

private:
    size_t delayCounter;
};


#endif //TPTALLER1_COIN_H
