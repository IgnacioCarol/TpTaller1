//
// Created by DANIELA CARRERO on 2020-10-26.
//

#ifndef TPTALLER1_TIMER_H
#define TPTALLER1_TIMER_H


#include <SDL2/SDL.h>
#include <tiff.h>

class Timer {
public:
    explicit Timer(int32 stopTime);
    void start();
    int32 getTimeSecond();
    bool isTimeOver();

private:
    int32 startTime = 0;
    int32 stopTime;
};


#endif //TPTALLER1_TIMER_H
