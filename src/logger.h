//
// Created by Daniel Bizari on 16/10/2020.
//
#include <iostream>
#include <fstream>

#ifndef TPTALLER1_LOGGER_H
#define TPTALLER1_LOGGER_H

typedef enum {ERROR, INFO, DEBUG} log_level;

class Logger {
    public:
        static Logger * getInstance();
        void setLogLevel(std::string logLevel);
        log_level getLogLevel();
        std::string info(std::string  msg);
        std::string debug(std::string msg);
        std::string error(std::string msg);
        virtual ~Logger();

    private:
        static Logger * instance;
        log_level       logLevel;
        std::ofstream   myFile;

        Logger();
        std::string getTimestamp();
        std::string writeMsg(std::string msg, std::string logLevel);
};


#endif //TPTALLER1_LOGGER_H
