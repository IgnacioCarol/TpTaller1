//
// Created by Daniel Bizari on 16/10/2020.
//
#include <iostream>
#ifndef TPTALLER1_LOGGER_H
#define TPTALLER1_LOGGER_H

typedef enum {INFO, DEBUG, ERROR} log_level;

class Logger {
    public:
        static Logger * getOrCreateLogger();
        void setLogLevel(std::string logLevel);
        void setPathToFile(std::string pathToFile);
        log_level getLogLevel();
        std::string getPathToFile();
        virtual ~Logger();

    private:
        static Logger * instance;
        log_level logLevel;
        std::string pathToFile;

        Logger();
};


#endif //TPTALLER1_LOGGER_H
