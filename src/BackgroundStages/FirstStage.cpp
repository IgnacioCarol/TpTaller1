//
// Created by nacho on 25/10/20.
//
#define PATH "Sprites/sprites_prueba/backgroundCompleto.png";

#include "FirstStage.h"

FirstStage::FirstStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->level = 1;
    this->backgroundPath = PATH;
    this->timer = new Timer(this->getLevelTime());
    setBackground();
    this->timer->start();
}

BackgroundStage * FirstStage::nextStage() {
    textureManager->clearFromTextureMap(BACKGROUND);
    textureManager->clearFromTextureMap(TEXT_WORLD_LEVEL_NUMBER_KEY);
    Game::Instance()->restartCharacters();
    Logger::getInstance()->debug("Stage changed into second stage");
    return new SecondStage(textureManager, renderer);
}
