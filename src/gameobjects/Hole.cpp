//
// Created by Daniel Bizari on 26/01/2021.
//

#include "Hole.h"

void Hole::init(int x, int y, std::string textureID) {
    GameObject::init(x, y, textureID);
    type = GOT_HOLE;
}

void Hole::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, this->width, this->height,
                                          0, renderer, SDL_FLIP_NONE);
}

int Hole::getHeight() {
    return this->height;
}

int Hole::getWidth() {
    return this->width;
}

void Hole::setDimensions(int width, int height) {
    this->width = width;
    this->height = height;
}
