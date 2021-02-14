#include "Score.h"


Score* Score::instance = nullptr;

Score* Score::getInstance() {
    if (instance == nullptr) {
        instance = new Score();
    }
    return instance;
}

Score::Score() {
    this->timer = new Timer(Config::getInstance()->getStage().score.time);
}

void Score::startLevelScore(int level) {
    currentLevel = level;
    this->timer->start();
}

bool Score::isShowScoreTimeOver() {
    return this->timer->isTimeOver();
}