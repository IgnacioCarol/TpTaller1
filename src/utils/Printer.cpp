//
// Created by DANIELA CARRERO on 2020-10-25.
//

#include <SDL_ttf.h>
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
        Logger::getInstance()->error("Printer initializer - Error initializing TTF");
        return;
    }
    Logger::getInstance()->info("Printer initializer - Success");
}

void Printer::print(SDL_Renderer *renderer, std::string text, SDL_Color color) {

}

TTF_Font* Printer::loadFont() {
    this->font = TTF_OpenFont(FONT_FILE_PATH, FONT_PTR_SIZE);
    return this->font;
}
