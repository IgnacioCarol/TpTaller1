//
// Created by milagros on 30/10/20.
//

#ifndef TPTALLER1_SURPRISEBLOCK_H
#define TPTALLER1_SURPRISEBLOCK_H

#include "Block.h"

//Image related definitions
#define SBWidth 313
#define SBHeight 327

//Animation related definitions
#define sBlockDelay 200
#define sBlockFrames 4
#define sBlockID "surpriseBlockSprites"


class SurpriseBlock : public Block {
public:
    SurpriseBlock();
    void init(int x, int y, std::string textureID, int currentFrame) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    virtual ~SurpriseBlock();

private:
    size_t delayCounter;
    int direction; //To know if it's changing to the darker color or the brigher.
};


#endif //TPTALLER1_SURPRISEBLOCK_H
