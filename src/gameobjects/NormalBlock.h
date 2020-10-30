#ifndef TPTALLER1_NORMALBLOCK_H
#define TPTALLER1_NORMALBLOCK_H

#include "Block.h"

//Image related definitions
#define NBWidth 88
#define NBHeight 86
#define nBlockID "normalBlock"

class NormalBlock : public Block {
public:
    NormalBlock();
    void init(int x, int y, std::string textureID, int currentFrame) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    virtual ~NormalBlock();
};


#endif //TPTALLER1_NORMALBLOCK_H
