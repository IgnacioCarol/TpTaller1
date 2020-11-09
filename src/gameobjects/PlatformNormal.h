#ifndef TPTALLER1_PLATFORMNORMAL_H
#define TPTALLER1_PLATFORMNORMAL_H
#include "GameObject.h"

class PlatformNormal : public GameObject {
public:
    PlatformNormal();
    ~PlatformNormal() override = default;
    void init(int x, int y, std::string textureID) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    void move() override {};
    int getHeight() override;

private:
    //Related to the image
    static const int NORMAL_BLOCK_WIDTH = 256;
    static const int NORMAL_BLOCK_HEIGHT = 256;

};


#endif //TPTALLER1_PLATFORMNORMAL_H
