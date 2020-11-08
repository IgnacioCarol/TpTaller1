//
// Created by DANIELA CARRERO on 2020-10-26.
//

#include "Timer.h"

Timer::Timer(int32 stopTime) {
    this->stopTime = stopTime;
}

void Timer::start() {
    this->startTime = SDL_GetTicks();
}

int32 Timer::getTimeSecond() {
    if (startTime > 0) {
        return this->stopTime - ((SDL_GetTicks() - startTime) / 1000);
    }
    return this->stopTime;
}

bool Timer::isTimeOver() {
    return getTimeSecond() <= 0;
}
