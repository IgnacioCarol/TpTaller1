//
// Created by lisandro on 28/11/20.
//

#ifndef TPTALLER1_PAUSED_H
#define TPTALLER1_PAUSED_H
#include "CharacterState.h"

class Paused: public CharacterState {
public:
    explicit Paused(bool disconnected = true, bool isPlayerBig = false);
    void move(const Uint8* currentKeyStates, Player* player) override;
    void changeState(const Uint8* currentKeyStates, Player* player) override;
    void draw(std::string ID, int xPosition, int yPosition, int imageWidth, int imageHeigth,
              SDL_Renderer* renderer, SDL_RendererFlip flip) override;
private:
    bool disconnected;
    static const int PAUSED_FRAME = 0;
};


#endif //TPTALLER1_PAUSED_H
