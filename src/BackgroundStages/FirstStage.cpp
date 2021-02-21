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
    Logger::getInstance()->info("Initializing first stage...");
    this->level = 1;
    this->backgroundPath = this->getLevelBackground().empty() ? DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND : this->getLevelBackground();
    this->timer = new Timer(this->getLevelTime());
    if(this->timer == nullptr) {
        Logger::getInstance()->error("Could not initialize timer for the stage");
        throw ConfigException("Could not initialize timer for the stage");
    }
    this->timer->start();
    Logger::getInstance()->info("First stage initialized");
}

FirstStage::FirstStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->level = 1;
    this->backgroundPath = this->getLevelBackground().empty() ? DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND : this->getLevelBackground();
    this->timer = new Timer(this->getLevelTime());
    setBackground();
    this->timer->start();
}

BackgroundStage * FirstStage::nextStage() {
    GameServer::Instance()->restartCharacters();
    Logger::getInstance()->debug("Stage changed into second stage");
    return new SecondStage();
}

Timer *FirstStage::getTimer() {
    if (timer == nullptr) {
        Logger::getInstance()->error("error, timer is null pointer");
        throw ConfigException("error, timer is null pointer");
    }
    return timer;
}
