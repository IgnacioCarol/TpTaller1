#include "PlatformNormal.h"

void PlatformNormal::init(int x, int y, std::string textureID) {
    GameObject::init(x, y, textureID);
    type = GOT_PLATFORM_NORMAL;
}

void PlatformNormal::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    TextureManager::Instance()->drawFrame(_textureID, xPosition - cameraX, yPosition, NORMAL_BLOCK_WIDTH, NORMAL_BLOCK_HEIGHT,
                                          0, renderer, SDL_FLIP_NONE);
}

int PlatformNormal::getHeight() {
    return NORMAL_BLOCK_HEIGHT / 4;
}

void PlatformNormal::collideWith(GameObject *go) {
    go->collideWith(this);
}

int PlatformNormal::getFloorPosition() {
    return - (getHeight() - 1);
}

int PlatformNormal::centerXPos() {
    return - 30;
}

int PlatformNormal::getWidth() {
    return NORMAL_BLOCK_WIDTH / 4;
}

int PlatformNormal::getMinHeightToIntersect() {
    return 65;
}