//
// Created by DANIELA CARRERO on 2020-10-25.
//

#include <SDL2/SDL_ttf.h>
#include "Printer.h"
#include "../logger/logger.h"

Printer *Printer::instance = nullptr;

Printer *Printer::getInstance() {
    if (Printer::instance == nullptr) {
        Printer::instance = new Printer();
    }

    return instance;
}

Printer::Printer() {
    if (TTF_Init() < 0 || loadFont() == NULL) {
        std::string error = "Printer initializer - Error initializing TTF. ";
        if (TTF_GetError() != NULL) {
            error + TTF_GetError();
        }
        Logger::getInstance()->error(error);
        return;
    }

    Logger::getInstance()->info("Printer initializer - Success");
}

TextTexture* Printer::getTextTexture(std::string text, SDL_Color color, SDL_Renderer *renderer) {
    TextTexture* textTexture;
    SDL_Surface* textSurface = TTF_RenderText_Solid(this->font, text.c_str(), color);
    SDL_Texture* texture = NULL;
    if (textSurface == NULL) {
        Logger::getInstance()->error("Couldn't print text, unable to initialize text surface: " +  text);
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == NULL) {
            Logger::getInstance()->error("Couldn't print text, unable to create texture from surface: " +  text);
        } else {
            textTexture = new TextTexture(texture, textSurface->w, textSurface->h); //TODO: Ver de liberar esta memoria
        }

        SDL_FreeSurface(textSurface);
    }

    return textTexture;
}

void Printer::render(TextTexture* textTexture, int x, int y, SDL_Renderer* renderer) {
    SDL_Rect renderQuad = { x, y, textTexture->width, textTexture->height};
    SDL_RenderCopyEx(renderer, textTexture->texture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}

void Printer::free(TextTexture* textTexture) {
    if (textTexture->texture != NULL) {
        SDL_DestroyTexture(textTexture->texture);
    }
}

TTF_Font* Printer::loadFont() {
    this->font = TTF_OpenFont(FONT_FILE_PATH, FONT_PTR_SIZE);
    return this->font;
}

TextTexture::TextTexture(SDL_Texture *pTexture, int w, int h) {
    this->texture = pTexture;
    this->width = w;
    this->height = h;
}

Printer::~Printer() {
    delete this->instance;
}
