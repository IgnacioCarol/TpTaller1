#include "Pipe.h"

void Pipe::init(int x, int y, std::string textureID) {
    this->type = GOT_PIPE;
    GameObject::init(x, y, textureID);
}

void Pipe::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, PIPE_WIDTH, PIPE_HEIGHT,
                                          0, renderer, SDL_FLIP_NONE);
}

int Pipe::getHeight() {
    return PIPE_HEIGHT;
}

int Pipe::getFloorPosition() {
    return -35;
}

int Pipe::getWidth() {
    return PIPE_WIDTH / 4;
}

void Pipe::collideWith(GameObject *go) {
    go->collideWith(this);
}

int Pipe::centerXPos() {
    return 10;
}