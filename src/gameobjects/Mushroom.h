//
// Created by lisandro on 16/2/21.
//

#ifndef TPTALLER1_MUSHROOM_H
#define TPTALLER1_MUSHROOM_H


#include "GameObject.h"

class Mushroom: public GameObject{
public:
    void init(int xPosition, int yPosition, std::string textureID) override;
    void collideWith(GameObject* go) override;
    void draw(SDL_Renderer* renderer, int cameraX, int cameraY) override;
    int getHeight() override;
    int getWidth() override;


private:
    static const int MUSHROOM_WIDTH = 256;
    static const int MUSHROOM_HEIGHT = 256;
    static const int MUSHROOM_FRAMES_AMOUNT = 4;
    static const int DELAY_MUSHROOM = 8;
    int delayCounter = 0;

};


#endif //TPTALLER1_MUSHROOM_H
