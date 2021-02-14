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

std::pair<int, int> PlatformNormal::getPosition() {
    return std::make_pair(xPosition, yPosition);
}

int PlatformNormal::getFloorPosition() {
    return -20;
}

int PlatformNormal::centerXPos() {
    return - getWidth() / 2;
}

int PlatformNormal::getWidth() {
    return NORMAL_BLOCK_WIDTH / 4;
}
