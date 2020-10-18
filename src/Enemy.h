#ifndef TPTALLER1_ENEMY_H
#define TPTALLER1_ENEMY_H


#include "GameObject.h"

class Enemy : public GameObject {
public:
    void init(size_t x, size_t y, size_t width, size_t height, std::string textureID, size_t frameAmount, currentFrame, currentRow) override;
    void draw() override;
    void update() override;
    void clean();
};


#endif //TPTALLER1_ENEMY_H
