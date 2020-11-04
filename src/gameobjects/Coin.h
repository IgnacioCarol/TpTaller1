#ifndef TPTALLER1_COIN_H
#define TPTALLER1_COIN_H

#include "GameObject.h"
#define coinDelay 30

//Image related definitions
#define CWidth 161
#define CHeight 164
#define defaultCoin "Sprites/Default/defaultCoins.png"
#define coinsID "coinsSprites" //Coins

//Animation related definitions
#define coinsFrames 4

class Coin : public GameObject {
    public:
        Coin();
        ~Coin() override;
        void  init(int x, int y, std::string fileName, std::string defaultImg,std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        void move() override {};
        std::string getFilePath() override;
        std::string getID() override;
        std::string getDefault() override;

private:
    size_t delayCounter;
};


#endif //TPTALLER1_COIN_H
