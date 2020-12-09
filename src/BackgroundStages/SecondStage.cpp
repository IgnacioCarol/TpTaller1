//
// Created by nacho on 25/10/20.
//
#ifdef __APPLE__
#include "../Server/GameServer.h"
#else
#include <src/Server/GameServer.h>
#endif
#include "SecondStage.h"

SecondStage::SecondStage() {
    this->level = 2;
    this->backgroundPath = this->getLevelBackground().empty() ? DEFAULT_STAGE_SECOND_LEVEL_BACKGROUND : this->getLevelBackground();
    this->timer = new Timer(this->getLevelTime());
    this->timer->start();
}

SecondStage::SecondStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->level = 2;
    this->backgroundPath = this->getLevelBackground().empty() ? DEFAULT_STAGE_SECOND_LEVEL_BACKGROUND : this->getLevelBackground();
    this->timer = new Timer(this->getLevelTime());
    setBackground();
    this->timer->start();
}

BackgroundStage * SecondStage::nextStage() {
    GameServer::Instance()->LEVEL_LIMIT = 7000;
    GameServer::Instance()->restartCharacters();
    Logger::getInstance()->debug("Stage changed into third stage");
    return new ThirdStage();
}