//
// Created by nacho on 25/10/20.
//
#define PATH "Sprites/sprites_prueba/world1-2.jpeg";
#include "SecondStage.h"

SecondStage::SecondStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->level = 2;
    this->backgroundPath = PATH;
    this->timer = new Timer(this->getLevelTime());
    setBackground();
    this->timer->start();
}

BackgroundStage * SecondStage::nextStage() {
    textureManager->clearFromTextureMap(BACKGROUND);
    textureManager->clearFromTextureMap(TEXT_WORLD_LEVEL_NUMBER_KEY);
    Game::Instance()->restartCharacters();
    Logger::getInstance()->debug("Stage changed into third stage");
    return new ThirdStage(textureManager, renderer);
}