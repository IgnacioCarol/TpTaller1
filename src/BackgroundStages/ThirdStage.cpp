#include <src/Server/GameServer.h>
#include "ThirdStage.h"

ThirdStage::ThirdStage() {
    this->level = 3;
    this->backgroundPath = this->getLevelBackground().empty() ? DEFAULT_STAGE_THIRD_LEVEL_BACKGROUND : this->getLevelBackground();
    this->timer = new Timer(this->getLevelTime());
    this->timer->start();
}

ThirdStage::ThirdStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->level = 3;
    this->backgroundPath = this->getLevelBackground().empty() ? DEFAULT_STAGE_THIRD_LEVEL_BACKGROUND : this->getLevelBackground();
    this->timer = new Timer(this->getLevelTime());
    setBackground();
    this->timer->start();
}

BackgroundStage *ThirdStage::nextStage() {
    GameServer::Instance()->gameOver();
    level = 0;
    delete timer;
    timer = nullptr;
    return this;
}
