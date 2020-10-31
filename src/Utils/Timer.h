//
// Created by DANIELA CARRERO on 2020-10-26.
//

#ifndef TPTALLER1_TIMER_H
#define TPTALLER1_TIMER_H


#include <SDL2/SDL.h>

class Timer {
public:
    explicit Timer(Uint32 stopTime);
    void start();
    Uint32 getTimeSecond();
    bool isTimeOver();

private:
    Uint32 startTime = 0;
    Uint32 stopTime;
};


#endif //TPTALLER1_TIMER_H
