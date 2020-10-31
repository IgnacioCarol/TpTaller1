//
// Created by nacho on 25/10/20.
//

#ifndef TPTALLER1_BACKGROUNDSTAGE_H
#define TPTALLER1_BACKGROUNDSTAGE_H


#include "../logger/logger.h"
#include "../TextureManager.h"
#include "../config/Constants.h"
#include "../Utils/Timer.h"
#include "../config/Config.h"

class BackgroundStage {
public:
    explicit BackgroundStage(TextureManager *pManager, SDL_Renderer *pRenderer);

    virtual BackgroundStage* nextStage();

    virtual int getWidth() const;

    bool renderLevel();

    bool renderTime();

protected:
    char* pathToCurrentImageStage; //Each stage should, and will have one for the constructor
    TextureManager* textureManager;
    SDL_Renderer* renderer;
    Logger* logger = Logger::getInstance();
    int imageWidth;
    const char* BACKGROUND = "BG";
    Timer* timer;
    int level;

    virtual bool setBackground() = 0;
    int getLevelTime();
};


#endif //TPTALLER1_BACKGROUNDSTAGE_H
