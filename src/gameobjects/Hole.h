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
    void collideWith(GameObject* go) override;
    int getFloorPosition() override;
    int centerXPos() override;
    int getLeftEdgePosition();
    int getRightEdgePosition();
private:
    int width;
    int level;
    int height;
    static const int LEFT_EDGE = 38; //The distance from the xPos to the left edge
    static const int RIGHT_EDGE = 126;
};


#endif //TPTALLER1_HOLE_H
