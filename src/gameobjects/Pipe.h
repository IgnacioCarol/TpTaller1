//
// Created by Daniel Bizari on 06/02/2021.
//

#ifndef TPTALLER1_PIPE_H
#define TPTALLER1_PIPE_H


#include "GameObject.h"

class Pipe : public GameObject {
public:
    ~Pipe() override = default;
    void init(int x, int y, std::string textureID) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    void move() override {};
    int getHeight() override;


private:
    //Related to the image
    static const int PIPE_WIDTH = 600;
    static const int PIPE_HEIGHT = 600;

    int getWidth() override;

    void collideWith(GameObject *go) override;

    int getFloorPosition() override;

    int centerXPos() override;
};


#endif //TPTALLER1_PIPE_H
