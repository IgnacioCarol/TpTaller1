//
// Created by DANIELA CARRERO on 2020-10-26.
//

#include "Timer.h"
#include "../logger/logger.h"

Timer::Timer(int stopTime) {
    this->stopTime = stopTime;
}

void Timer::start() {
    int time = clock();
    Logger::getInstance()->info("Starting timer with: " + std::to_string(time));
    this->startTime = time;
    started = true;
}

int Timer::getTimeSecond() {
    if (started && startTime > 0) {
        return this->stopTime - ((float)(clock() - startTime) / CLOCKS_PER_SEC);
    }
    return this->stopTime;
}

bool Timer::isTimeOver() {
    return getTimeSecond() <= 0;
}

Timer::~Timer() {
}
