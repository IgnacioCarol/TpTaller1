//
// Created by nacho on 25/10/20.
//
#ifdef __APPLE__
#include "../Server/GameServer.h"
#else
#include <src/Server/GameServer.h>
#endif

#include "FirstStage.h"

FirstStage::FirstStage() {
    Logger::getInstance()->debug("Initializing first stage...");
    this->level = 1;
    this->backgroundPath = this->getLevelBackground().empty() ? DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND : this->getLevelBackground();
    this->timer = new Timer(this->getLevelTime());
    this->timer->start();
    Logger::getInstance()->debug("First stage initialized");
}

FirstStage::FirstStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->level = 1;
    this->backgroundPath = this->getLevelBackground().empty() ? DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND : this->getLevelBackground();
    this->timer = new Timer(this->getLevelTime());
    setBackground();
    this->timer->start();
}

BackgroundStage * FirstStage::nextStage() {
    GameServer::Instance()->LEVEL_LIMIT = 3800;
    GameServer::Instance()->restartCharacters();
    Logger::getInstance()->debug("Stage changed into second stage");
    return new SecondStage();
}
