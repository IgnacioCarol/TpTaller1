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

void Hole::setLevel(int level) {
    this->level = level;
}

void Hole::collideWith(GameObject *go) {
    /*go.fall();
    if (!go->isAtScene()){ //Significa que cayo y desaparecio de la pantalla
        go.dieFalling();
    }*/
}
