//
// Created by DANIELA CARRERO on 2020-10-26.
//

#include "Timer.h"

Timer::Timer(Uint32 stopSeconds) {
    this->stopTime = stopSeconds;
}

void Timer::start() {
    this->startTime = SDL_GetTicks();
}

Uint32 Timer::getTimeSecond() {
    if (startTime != 0) {
        return this->stopTime - ((SDL_GetTicks() - startTime) / 1000);
    }
    return this->stopTime;
}
