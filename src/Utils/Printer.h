//
// Created by DANIELA CARRERO on 2020-10-25.
//

#ifndef TPTALLER1_PRINTER_H
#define TPTALLER1_PRINTER_H
#ifdef __APPLE__
#define FONT_FILE_PATH "../resources/fonts/SuperMario256.ttf"
#include "../logger/logger.h"
#else
#define FONT_FILE_PATH "resources/fonts/SuperMario256.ttf"
#include "src/logger/logger.h"
#endif
#define FONT_PTR_SIZE 20

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>

struct TextTexture {
    TextTexture(SDL_Texture *pTexture, int w, int h);

    SDL_Texture* texture = NULL;
    int width = 0;
    int height = 0;
};

class Printer {
public:
    static Printer *getInstance();
    TextTexture* getTextTexture(std::string text, SDL_Color color, SDL_Renderer* renderer);
    void free(TextTexture* texture); //TODO: Ver si es necesario
    void render(TextTexture* texture, int x, int y, SDL_Renderer* renderer);
    virtual ~Printer() { };
private:
    static Printer *instance;
    TTF_Font *font;

    Printer();
    TTF_Font* loadFont();
};


#endif //TPTALLER1_PRINTER_H
