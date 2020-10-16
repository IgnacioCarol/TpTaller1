//
// Created by Daniel Bizari on 16/10/2020.
//
#include <iostream>
#ifndef TPTALLER1_LOGGER_H
#define TPTALLER1_LOGGER_H

typedef enum {INFO, DEBUG, ERROR} log_level;

class Logger {
    public:
        Logger();
        void setLogLevel(log_level logLevel);
        void setPathToFile(std::string pathToFile);
        log_level getLogLevel();
        std::string getPathToFile();

    private:
        log_level logLevel;
        std::string pathToFile;
};


#endif //TPTALLER1_LOGGER_H
