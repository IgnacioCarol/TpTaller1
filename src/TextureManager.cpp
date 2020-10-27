//
// Created by lisandro on 23/10/20.
//

#include "TextureManager.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../src/logger/logger.h"
static const char *const BACKGROUND = "BG";
static const int CURRENT_ROW = 0; //The sprite sheet always has one row

TextureManager* TextureManager::instance = 0;

bool TextureManager::load(const std::string& fileName, const std::string& ID, SDL_Renderer *imageRenderer) {
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    if (!tempSurface){
        Logger::getInstance() -> error("Error: couldn't load the image\n");
        return false;
    }

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(imageRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (imageTexture != 0){
        textureMap[ID] = imageTexture;
        return true;
    }
    return false;
}

void TextureManager::draw(std::string ID, int x, int y, int width, int height, SDL_Renderer *renderer,
                          SDL_RendererFlip flip) {
    SDL_Rect  srcRect; //Aca defino size de la imagen
    SDL_Rect destRect; //Aca donde va a ir, se mapea para ajustarse el tamanio

    SDL_Texture* texture = textureMap[ID];
    SDL_QueryTexture(texture, NULL, NULL, &srcRect.w, &srcRect.h);

    srcRect.x = 0;
    srcRect.y = 0; //Tomo parte superior de la imagen
    if (width == 1) {
        destRect.w = srcRect.w / 4;
        destRect.h = srcRect.h / 4;
    }
    else{
        srcRect.w = width;
        srcRect.h = height;
        destRect.w = srcRect.w / 4;
        destRect.h = srcRect.h / 4;
    }
    destRect.x = x;
    destRect.y = y;

    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0, 0, flip);

}

void TextureManager::drawBackground(int width, int height, SDL_Renderer *renderer) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    SDL_Texture* texture = textureMap[BACKGROUND];
    SDL_QueryTexture(texture, NULL, NULL, &srcRect.w, &srcRect.h);

    srcRect.x = 100;  //vas cambiando este vaor siempre
    destRect.x = destRect.y = srcRect.y = 0;

    destRect.w = srcRect.w = 800;
    destRect.h = srcRect.h = 600;

    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
}

void TextureManager::drawBackgroundWithCamera(int width, int height, SDL_Renderer *renderer, SDL_Rect* clip) {
    {
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { 0, 0, width, height };
        SDL_Texture* texture = textureMap[BACKGROUND];
        //Set clip rendering dimensions
        if( clip != NULL )
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        //Render to screen
        SDL_RenderCopyEx( renderer, texture, clip, &renderQuad, 0, 0, SDL_FLIP_NONE );
    }
}

void
TextureManager::drawFrame(std::string ID, int x, int y, int width, int height, int currentFrame, SDL_Renderer *renderer,
                          SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = currentFrame;
    srcRect.y = CURRENT_ROW;
    srcRect.w = width;
    srcRect.h = height;

    destRect.x = x;
    destRect.y = y;
    destRect.w = width / 4;
    destRect.h = height / 4;
    SDL_RenderCopyEx(renderer, textureMap[ID], &srcRect, &destRect, 0, 0, flip);
    //Creo que esta se usa para elegir bien la posicion del sprite
}

void TextureManager::clearTextureMap()
{
    textureMap.clear();
}

void TextureManager::clearFromTextureMap(std::string id)
{
    textureMap.erase(id);
}
