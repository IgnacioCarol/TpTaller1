//
// Created by DANIELA CARRERO on 2020-11-02.
//

#define PATH "Sprites/sprites_prueba/level3.png";
#include "ThirdStage.h"

ThirdStage::ThirdStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->level = 3;
    this->backgroundPath = PATH;
    this->timer = new Timer(this->getLevelTime());
    setBackground();
    this->timer->start();
}

BackgroundStage *ThirdStage::nextStage() {
    textureManager->clearFromTextureMap(BACKGROUND);
    textureManager->clearFromTextureMap(TEXT_WORLD_LEVEL_NUMBER_KEY);
    Game::Instance()->gameOver();
    return this;
}
