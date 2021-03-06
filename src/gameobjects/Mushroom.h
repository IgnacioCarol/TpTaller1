//
// Created by lisandro on 16/2/21.
//

#ifndef TPTALLER1_MUSHROOM_H
#define TPTALLER1_MUSHROOM_H


#include "GameObject.h"
#include "../Utils/MusicManager.h"
#include "../config/Constants.h"
#include "../Utils/Vector.h"

class Mushroom: public GameObject{
public:
    void init(int xPosition, int yPosition, std::string textureID) override;
    void collideWith(GameObject* go) override;
    void draw(SDL_Renderer* renderer, int cameraX, int cameraY) override;
    int getHeight() override;
    int getWidth() override;
    void hide() override;
    void unhide() override;
    std::string getState() override;
    void setState(std::string newState) override;
    void changeState(std::string newState);
    void move() override;
    Vector* getBottomRightBorder();
    Vector* getTopLeftBorder();
    int getFloorPosition() override;
    int centerXPos() override;



private:
    static const int MUSHROOM_WIDTH = 256;
    static const int MUSHROOM_HEIGHT = 241;
    static const int MUSHROOM_FRAMES_AMOUNT = 4;
    static const int DELAY_MUSHROOM = 8;
    int delayCounter = 0;
    std::string stateType = "HIDE";

};


#endif //TPTALLER1_MUSHROOM_H
