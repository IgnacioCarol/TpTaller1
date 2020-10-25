//
// Created by nacho on 25/10/20.
//

#include "BackgroundStage.h"

BackgroundStage::BackgroundStage(TextureManager *pManager, SDL_Renderer *pRenderer) {
    textureManager = pManager;
    renderer = pRenderer;
}