//
// Created by Daniel Bizari on 26/10/2020.
//

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
    return NORMAL_BLOCK_HEIGHT;
}

void PlatformNormal::collideWith(Player *player) {
    std::pair<int, int> position = player->getPosition();
    int xPlayer = position.first;
    int yPlayer = position.second;
    if(xPlayer > xPosition && xPlayer < (xPosition + NORMAL_BLOCK_WIDTH) && yPlayer >= (yPosition + NORMAL_BLOCK_HEIGHT))
        player->setPosition(xPlayer, yPosition + NORMAL_BLOCK_HEIGHT); //TODO chequear si queda bien graficamente
}
