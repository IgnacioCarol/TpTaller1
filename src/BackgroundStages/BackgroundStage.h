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
#include <string.h>

class BackgroundStage {
public:
    explicit BackgroundStage(TextureManager *pManager, SDL_Renderer *pRenderer);
    ~BackgroundStage();
    virtual BackgroundStage* nextStage();
    int getWidth() const;
    bool renderLevel();
    bool renderTime();
    bool isTimeOver();
    int getLevel();
    std::string getLevelBackground();

protected:
    std::string backgroundPath;
    TextureManager* textureManager;
    SDL_Renderer* renderer;
    Logger* logger = Logger::getInstance();
    int imageWidth;
    const char* BACKGROUND = "BG";
    Timer* timer;
    int level;

    bool setBackground();
    int getLevelTime();
};


#endif //TPTALLER1_BACKGROUNDSTAGE_H
