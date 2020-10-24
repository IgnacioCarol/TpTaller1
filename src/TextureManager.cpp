//
// Created by lisandro on 23/10/20.
//

#include "TextureManager.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

TextureManager* TextureManager::instance = 0;

bool TextureManager::load(std::string fileName, std::string ID, SDL_Renderer *imageRenderer) {
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    if (!tempSurface){
        printf("Falle cargando la imagen\n");
        return false;
    }

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(imageRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (imageTexture != 0){
        printf("Success creating the texture\n");
        textureMap[ID] = imageTexture;
        return true;
    }
    return false;
}

void TextureManager::draw(std::string ID, int x, int y, int width, int height, SDL_Renderer *renderer,
                          SDL_RendererFlip flip) {
    //deberia tener un tamanio fijo para la imagen y otro para el backgroun, back 800x600 la imagen
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

    SDL_Texture* texture = textureMap["BG"];
    SDL_QueryTexture(texture, NULL, NULL, &srcRect.w, &srcRect.h);

    srcRect.x = 100;  //vas cambiando este vaor siempre
    destRect.x = destRect.y = srcRect.y = 0;

    destRect.w = srcRect.w = 800;
    destRect.h = srcRect.h = 600;

    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
}

void TextureManager::drawFrame(std::string ID, int x, int y, int width, int height, int currentRow, int currentFrame,
                               SDL_Renderer *renderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;
    /*srcRect.x = width * currentFrame;
    srcRect.y = height * (currentRow - 1);
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;
    destRect.x = x;
    destRect.y = y;
     */
    srcRect.x = currentRow;
    srcRect.y = currentFrame;
    srcRect.w = width;
    srcRect.h = height;
    destRect.x = x;
    destRect.y = y;
    destRect.w = width / 4;
    destRect.h = height / 4;
    SDL_RenderCopyEx(renderer, textureMap[ID], &srcRect, &destRect, 0, 0, flip);
    //Creo que esta se usa para elegir bien la posicion del sprite
}

void TextureManager::destroy(){
    SDL_DestroyTexture(textureMap["dino"]);
    SDL_DestroyTexture(textureMap["BG"]);
    SDL_DestroyTexture(textureMap["dog"]);
    SDL_DestroyTexture(textureMap["runDog"]);
}