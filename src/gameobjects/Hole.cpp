//
// Created by Daniel Bizari on 26/01/2021.
//

#include "Hole.h"

void Hole::init(int x, int y, std::string textureID) {
    // Set level must be called always before init
    if (level == 2) {
        textureID = "hole-black";
    }

    this->type = GOT_HOLE;
    GameObject::init(x, y, textureID);
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

void Hole::collideWith(GameObject *go) {
    if(go->getXPosition() >= xPosition - LEFT_EDGE && go->getXPosition() <= xPosition + RIGHT_EDGE) {
        go->fall();
        go->collideWith(this);
        go->dieFalling();
    }
}

void Hole::setLevel(int level) {
    this->level = level;
}

int Hole::getFloorPosition() {
    return -62;
    //return -50; //Lo dejo asi para debugguear otras cosas
}

int Hole::centerXPos() {
    return 50;
}

int Hole::getLeftEdgePosition() {
    return xPosition - LEFT_EDGE;
}

int Hole::getRightEdgePosition() {
    return xPosition + RIGHT_EDGE;
}
