//
// Created by nacho on 25/10/20.
//
const static char* PATH = "Sprites/sprites_prueba/backgroundCompleto.png";

#include <src/Game.h>
#include "FirstStage.h"
#include "BackgroundStage.h"
#include "SecondStage.h"

FirstStage::FirstStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    setBackground();
}

bool FirstStage::setBackground() {
    bool success =  textureManager-> load(PATH, BACKGROUND, renderer);
    if (!success) {
        printf("Image not found at %s\n", PATH);
        return false;
    }
    SDL_QueryTexture(textureManager->getTextureMap()[BACKGROUND], NULL, NULL, &imageWidth, NULL);
    return true;
}

int FirstStage::getWidth() const {
    return imageWidth;
}

BackgroundStage * FirstStage::nextStage() {
    textureManager->clearFromTextureMap(BACKGROUND);
    Game::Instance()->restartCharacters();
    return new SecondStage(textureManager, renderer);
}
