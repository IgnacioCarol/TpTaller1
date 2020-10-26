//
// Created by Daniel Bizari on 26/10/2020.
//

#ifndef TPTALLER1_COIN_H
#define TPTALLER1_COIN_H

#include "GameObject.h"

class Coin : public GameObject {
    public:
        Coin();
        void init(int x, int y, std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        virtual ~Coin();
};


#endif //TPTALLER1_COIN_H
