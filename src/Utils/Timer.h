//
// Created by DANIELA CARRERO on 2020-10-26.
//

#ifndef TPTALLER1_TIMER_H
#define TPTALLER1_TIMER_H

#include <ctime>

class Timer {
public:
    explicit Timer(int stopTime);
    void start();
    int getTimeSecond();
    bool isTimeOver();
    virtual ~Timer();
private:
    clock_t startTime = 0;
    int stopTime;

    bool started = false;
};


#endif //TPTALLER1_TIMER_H
