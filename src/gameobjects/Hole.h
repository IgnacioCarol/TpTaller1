//
// Created by Daniel Bizari on 26/01/2021.
//

#ifndef TPTALLER1_HOLE_H
#define TPTALLER1_HOLE_H


#include "GameObject.h"

class Hole : public GameObject {
public:
    ~Hole() override = default;
    void init(int x, int y, std::string textureID) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    void move() override {};
    int getHeight() override;
    int getWidth() override;
    void setDimensions(int width, int height);
    void setLevel(int level);
private:
    int width;
    int level;
    int height;
};


#endif //TPTALLER1_HOLE_H
