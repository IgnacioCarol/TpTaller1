//
// Created by Daniel Bizari on 26/10/2020.
//

#ifndef TPTALLER1_ENEMYMUSHROOM_H
#define TPTALLER1_ENEMYMUSHROOM_H

#include "GameObject.h"

class EnemyMushroom : public GameObject {
    public:
        EnemyMushroom();
        void init(int x, int y, std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        virtual ~EnemyMushroom();
};


#endif //TPTALLER1_ENEMYMUSHROOM_H
