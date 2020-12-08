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
    BackgroundStage() = default;
    explicit BackgroundStage(TextureManager *pManager, SDL_Renderer *pRenderer);
    ~BackgroundStage();
    virtual BackgroundStage* nextStage();
    int getWidth() const;
    bool renderLevel();
    bool renderTime();
    void renderDefaultBackground();
    bool isTimeOver();
    int getLevel();
    void setCurrentTime(int currentTime);
    void setLevel(int level);
    void setBackgroundID(std::string bgID);
    void isDefaultBackground(bool defaultBackground);
    void renderBackground(SDL_Rect *camera);
    Timer* getTimer();
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
    int currentTime;
    std::string bgID;
    bool defaultBackground;

    bool setBackground();
    int getLevelTime();
};


#endif //TPTALLER1_BACKGROUNDSTAGE_H
