#include "BackgroundStage.h"

BackgroundStage::BackgroundStage(TextureManager *pManager, SDL_Renderer *pRenderer) {
    textureManager = pManager;
    renderer = pRenderer;
}

BackgroundStage::~BackgroundStage() {
}

BackgroundStage *BackgroundStage::nextStage() {
    return nullptr;
}

bool BackgroundStage::renderLevel() {
    if (level != 0) {
        textureManager->printText(TEXT_WORLD_LEVEL_LABEL_KEY, TEXT_WORLD_LEVEL_LABEL_XPOS, TEXT_WORLD_LEVEL_LABEL_YPOS, renderer);
        bool success = textureManager->loadText(TEXT_WORLD_LEVEL_NUMBER_KEY, std::to_string(level), WHITE_COLOR, renderer);
        if (!success) {
            logger->error("Error loading level text in level: " + std::to_string(level));
            return false;
        }
        textureManager->printText(TEXT_WORLD_LEVEL_NUMBER_KEY, TEXT_WORLD_LEVEL_NUMBER_XPOS, TEXT_WORLD_LEVEL_NUMBER_YPOS, renderer);
    }
    return true;
}

bool BackgroundStage::renderTime() {
    textureManager->printText(TEXT_TIMER_LABEL_KEY, TEXT_TIMER_LABEL_XPOS, TEXT_TIMER_LABEL_YPOS, renderer);
    bool success = textureManager->loadText(TEXT_TIMER_VALUE_KEY, std::to_string(currentTime), WHITE_COLOR, renderer);
    if (!success) {
        logger->error("Error loading timer value in level: " + std::to_string(level));
        return false;
    }
    textureManager->printText(TEXT_TIMER_VALUE_KEY, TEXT_TIMER_VALUE_XPOS, TEXT_TIMER_VALUE_YPOS, renderer);
    return true;
}

void BackgroundStage::renderDefaultBackground() {
    textureManager->printText(TEXT_DEFAULT_BACKGROUND_KEY, TEXT_DEFAULT_BACKGROUND_XPOS, TEXT_DEFAULT_BACKGROUND_YPOS, renderer);
}

bool BackgroundStage::isTimeOver() {
    return this->timer != nullptr && this->timer->isTimeOver();
}

int BackgroundStage::getLevelTime() {
    int time;
    try {
        time = Config::getInstance()->getLevel(this->level).time;
    } catch (ConfigException &ex) {
        time = DEFAULT_STAGE_LEVEL_TIME;
    }
    return time;
}

std::string BackgroundStage::getLevelBackground() {
    std::string background;
    try {
        background = Config::getInstance()->getLevel(this->level).background;
    } catch (ConfigException &ex) {
        Logger::getInstance()->error("Couldn't find background for level " + std::to_string(level));
    }
    return background;
}

bool BackgroundStage::setBackground() {
    bool success =  textureManager-> load(backgroundPath, BACKGROUND, renderer);
    if (!success) {
        string error = "error image not found at ";
        error.append(backgroundPath);
        logger->error(error);
        return false;
    }
    SDL_QueryTexture(textureManager->getTextureMap()[BACKGROUND], NULL, NULL, &imageWidth, NULL);
    return true;
}

int BackgroundStage::getWidth() const {
    return imageWidth;
}

int BackgroundStage::getLevel() {
    return level;
}

void BackgroundStage::setCurrentTime(int currentTime) {
    this -> currentTime = currentTime;
}

void BackgroundStage::setLevel(int level) {
    this -> level = level;
}

void BackgroundStage::setBackgroundID(std::string bgID) {
    this -> bgID = bgID;
}


void BackgroundStage::renderBackground(SDL_Rect* camera) {
    textureManager -> drawBackgroundWithCamera(800, 600, bgID, renderer, camera);
    renderTime();
    renderLevel();
    renderSoundMusicState();
    if (defaultBackground){
        renderDefaultBackground();
    }
}

void BackgroundStage::isDefaultBackground(bool defaultBackground) {
    this -> defaultBackground = defaultBackground;
}

int BackgroundStage::getLevelLimit() {
    return LEVEL_LIMIT;
}

void BackgroundStage::renderSoundMusicState() {
    if (MusicManager::Instance()->areSoundEffectsOff()){
        textureManager->printText(TEXT_MUSIC, TEXT_MUSIC_X_POS, TEXT_MUSIC_Y_POS,renderer);
        textureManager->printText(TEXT_OFF, TEXT_MUSIC_X_POS + 100, TEXT_MUSIC_Y_POS,renderer);

        textureManager->printText(TEXT_SOUND, TEXT_SOUND_X_POS, TEXT_SOUND_Y_POS,renderer);
        textureManager->printText(TEXT_OFF, TEXT_SOUND_X_POS + 100, TEXT_SOUND_Y_POS,renderer);
    }
    else if (MusicManager::Instance()->isMusicPaused()){
        textureManager->printText(TEXT_MUSIC, TEXT_MUSIC_X_POS, TEXT_MUSIC_Y_POS,renderer);
        textureManager->printText(TEXT_OFF, TEXT_MUSIC_X_POS + 100, TEXT_MUSIC_Y_POS,renderer);
    }
    else if (MusicManager::Instance()->areSoundsMuted()){
        textureManager->printText(TEXT_SOUND, TEXT_SOUND_X_POS, TEXT_MUSIC_Y_POS,renderer);
        textureManager->printText(TEXT_OFF, TEXT_SOUND_X_POS + 100, TEXT_MUSIC_Y_POS,renderer);
    }
}

Timer *BackgroundStage::getTimer() {
    return nullptr;
}
