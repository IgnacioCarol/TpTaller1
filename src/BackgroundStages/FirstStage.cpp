//
// Created by nacho on 25/10/20.
//
const static char* PATH = "Sprites/sprites_prueba/backgroundCompleto.png";

#include "FirstStage.h"

FirstStage::FirstStage(TextureManager *pManager, SDL_Renderer *pRenderer) : BackgroundStage(pManager, pRenderer) {
    this->level = 1;
    this->timer = new Timer(300); //TODO obtener de la config
    setBackground();
    renderLevel();
    this->timer->start();
    renderTime();
}

FirstStage::~FirstStage() {
    delete this->timer;
}

bool FirstStage::setBackground() {
    bool success =  textureManager-> load(PATH, BACKGROUND, renderer);
    if (!success) {
        string error = "error image not found at ";
        error.append(PATH);
        logger->error(error);
        return false;
    }
    SDL_QueryTexture(textureManager->getTextureMap()[BACKGROUND], NULL, NULL, &imageWidth, NULL);
    return true;
}

int FirstStage::getWidth() const {
    return imageWidth;
}

BackgroundStage * FirstStage::nextStage() {
    textureManager->clearFromTextureMap(BACKGROUND);
    textureManager->clearFromTextureMap(TEXT_WORLD_LEVEL_NUMBER_KEY);
    Game::Instance()->restartCharacters();
    Logger::getInstance()->debug("Stage changed into second stage");
    return new SecondStage(textureManager, renderer);
}
