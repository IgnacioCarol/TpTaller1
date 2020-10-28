//
// Created by nacho on 25/10/20.
//

#include "BackgroundStage.h"

BackgroundStage::BackgroundStage(TextureManager *pManager, SDL_Renderer *pRenderer) {
    textureManager = pManager;
    renderer = pRenderer;
}

BackgroundStage *BackgroundStage::nextStage() {
    return nullptr;
}

int BackgroundStage::getWidth() const {
    return 0;
}

bool BackgroundStage::setBackground() {
    return false;
}

bool BackgroundStage::renderLevel() {
    bool success = textureManager->loadText(TEXT_WORLD_LEVEL_NUMBER_KEY, std::to_string(level), WHITE_COLOR, renderer);
    if (!success) {
        logger->error("Error loading level text in level: " + std::to_string(level));
        return false;
    }
    textureManager->printText(TEXT_WORLD_LEVEL_NUMBER_KEY, TEXT_WORLD_LEVEL_NUMBER_XPOS, TEXT_WORLD_LEVEL_NUMBER_YPOS, renderer);
    return true;
}

bool BackgroundStage::renderTime() {
    bool success = textureManager->loadText(TEXT_TIMER_VALUE_KEY, std::to_string(timer->getTimeSecond()), WHITE_COLOR, renderer);
    if (!success) {
        logger->error("Error loading timer value in level: " + std::to_string(level));
        return false;
    }
    textureManager->printText(TEXT_TIMER_VALUE_KEY, TEXT_TIMER_VALUE_XPOS, TEXT_TIMER_VALUE_YPOS, renderer);
    return true;
}

