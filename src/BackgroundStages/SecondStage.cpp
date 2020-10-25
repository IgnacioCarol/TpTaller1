//
// Created by nacho on 25/10/20.
//
const static char* PATH = "Sprites/sprites_prueba/world1-2.jpeg";
#include "SecondStage.h"
#include "Game.h"

SecondStage::SecondStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->setBackground();
}

bool BackgroundStage::setBackground() {
    bool success =  textureManager-> load(PATH, BACKGROUND, renderer);
    if (!success) {
        printf("Image not found at %s\n", PATH);
        return false;
    }
    SDL_QueryTexture(textureManager->getTextureMap()[BACKGROUND], NULL, NULL, &imageWidth, NULL);
    return true;
}

int BackgroundStage::getWidth() const {
    return imageWidth;
}

BackgroundStage * BackgroundStage::nextStage() {
    textureManager->clearFromTextureMap(BACKGROUND);
    Game::Instance()->gameOver();
    return nullptr;
}