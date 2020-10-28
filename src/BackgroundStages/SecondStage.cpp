//
// Created by nacho on 25/10/20.
//
const static char* PATH = "Sprites/sprites_prueba/world1-2.jpeg";
#include "SecondStage.h"
#include "../Game.h"

SecondStage::SecondStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->level = 2;
    this->timer = new Timer(300); //TODO sacar de la config
    setBackground();
    renderLevel();
    this->timer->start();
    renderTime();
}

SecondStage::~SecondStage() {
    delete this->timer;
}

bool SecondStage::setBackground() {
    bool success =  textureManager-> load(PATH, BACKGROUND, renderer);
    if (!success) {
        string error = "error image not found at ";
        error.append(PATH);
        logger->error(error);
        return false;
    }
    SDL_QueryTexture(textureManager->getTextureMap()[BACKGROUND], NULL, NULL, &imageWidth, NULL);
    return true;
}

int SecondStage::getWidth() const {
    return imageWidth;
}

BackgroundStage * SecondStage::nextStage() {
    textureManager->clearFromTextureMap(BACKGROUND);
    textureManager->clearFromTextureMap(TEXT_WORLD_LEVEL_NUMBER_KEY);
    Game::Instance()->gameOver();
    return nullptr;
}