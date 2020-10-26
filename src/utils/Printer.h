//
// Created by DANIELA CARRERO on 2020-10-25.
//

#ifndef TPTALLER1_PRINTER_H
#define TPTALLER1_PRINTER_H

#define FONT_FILE_PATH "../resources/fonts/SuperMario256.ttf"
#define FONT_PTR_SIZE 20

#include <SDL2/SDL.h>
#include <string>

class Printer {
public:
    static Printer *getInstance();
    void print(SDL_Renderer* renderer, std::string text, SDL_Color color);
    virtual ~Printer() { };
private:
    static Printer *instance;
    TTF_Font *font;
    TTF_Font* loadFont();
    Printer();
};


#endif //TPTALLER1_PRINTER_H
