//
// Created by Daniel Bizari on 26/10/2020.
//

#ifndef TPTALLER1_ENEMYTURTLE_H
#define TPTALLER1_ENEMYTURTLE_H

#include "GameObject.h"

class EnemyTurtle : public GameObject {
    public:
        EnemyTurtle();
        void init(int x, int y, std::string textureID, int currentFrame) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        virtual ~EnemyTurtle();

};


#endif //TPTALLER1_ENEMYTURTLE_H
