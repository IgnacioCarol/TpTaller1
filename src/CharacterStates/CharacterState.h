//
// Created by lisandro on 25/10/20.
//

#ifndef TPTALLER1_CHARACTERSTATE_H
#define TPTALLER1_CHARACTERSTATE_H


#include <SDL2/SDL_render.h>
#include <string>
class Player;
class Enemy;

class CharacterState {
public:
    explicit CharacterState(int currentFrame, int framesAmount);

    explicit CharacterState();

    virtual void draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth,
                      SDL_Renderer* renderer, SDL_RendererFlip flip);

    virtual void move(const Uint8* currentKeyStates, Player* player); //For the players

    virtual void move(Enemy* enemy); //For the enemies

    virtual void changeState(const Uint8* currentKeyStates, Player* player);

    int getFramesAmount();

    std::string getStateType();

protected:
    static const int FRAMES_AMOUNT_PLAYER = 7;

    static const int ITER_TIMES = 4;

    std::string stateType;
    int currentFrame;
    int framesAmount; //Amount of pictures in the sprite sheet
    int contAux = 0;
    bool isPlayerBig;
};
#endif //TPTALLER1_CHARACTERSTATE_H