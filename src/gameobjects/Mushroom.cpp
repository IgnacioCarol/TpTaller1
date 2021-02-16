//
// Created by lisandro on 16/2/21.
//

#include "Mushroom.h"

void Mushroom::init(int xPosition, int yPosition, std::string textureID) {
    GameObject::init(xPosition, yPosition, textureID);
    type = GOT_MUSHROOM;
    //points = POINTS_COIN; chequear si da o no puntos
}

void Mushroom::collideWith(GameObject *go) {
    go->collideWith(this);
}

void Mushroom::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    //ToDo una vez que ande esto agregar condicion de que sea visible
    if (delayCounter == DELAY_MUSHROOM && _currentFrame < MUSHROOM_FRAMES_AMOUNT) {
        _currentFrame += 1;
        delayCounter = 0;
    }
    else delayCounter++;

    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, MUSHROOM_WIDTH, MUSHROOM_HEIGHT,
                                          MUSHROOM_WIDTH * _currentFrame, renderer, SDL_FLIP_NONE);
}

int Mushroom::getHeight() {
    return MUSHROOM_HEIGHT;
}

int Mushroom::getWidth() {
    return MUSHROOM_WIDTH;
}
