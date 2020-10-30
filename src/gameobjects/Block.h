#ifndef TPTALLER1_BLOCK_H
#define TPTALLER1_BLOCK_H
#include "GameObject.h"


class Block : public GameObject {
public:
    void init(int x, int y, std::string textureID, int currentFrame) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override = 0;
};


#endif //TPTALLER1_BLOCK_H
