#ifndef TPTALLER1_SCORE_H
#define TPTALLER1_SCORE_H


#include "Timer.h"
#include "../config/Config.h"

class Score {
public:
    static Score* getInstance();

    void startLevelScore(int level);
    bool isShowScoreTimeOver();
private:
    Score();
    static Score* instance;

    Timer* timer;
    int currentLevel;
};


#endif //TPTALLER1_SCORE_H
