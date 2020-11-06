#include "ThirdStage.h"

ThirdStage::ThirdStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->level = 3;
    this->backgroundPath = this->getLevelBackground().empty() ? DEFAULT_STAGE_THIRD_LEVEL_BACKGROUND : this->getLevelBackground();
    this->timer = new Timer(this->getLevelTime());
    setBackground();
    this->timer->start();
}

BackgroundStage *ThirdStage::nextStage() {
    textureManager->clearFromTextureMap(BACKGROUND);
    textureManager->clearFromTextureMap(TEXT_WORLD_LEVEL_NUMBER_KEY);
    Game::Instance()->gameOver();
    level = 0;
    timer = nullptr;
    return this;
}
