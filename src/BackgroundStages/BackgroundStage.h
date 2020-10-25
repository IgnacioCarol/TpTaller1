//
// Created by nacho on 25/10/20.
//

#ifndef TPTALLER1_BACKGROUNDSTAGE_H
#define TPTALLER1_BACKGROUNDSTAGE_H


#include "../logger/logger.h"
#include "../TextureManager.h"

class BackgroundStage {
public:
    explicit BackgroundStage(TextureManager *pManager, SDL_Renderer *pRenderer);

    virtual BackgroundStage* nextStage();

    virtual int getWidth() const;

protected:
    char* pathToCurrentImageStage; //Each stage should, and will have one for the constructor
    TextureManager* textureManager;
    SDL_Renderer* renderer;
    Logger* logger = Logger::getInstance();
    virtual bool setBackground();
    int imageWidth;
    const char* BACKGROUND = "BG";
};


#endif //TPTALLER1_BACKGROUNDSTAGE_H
