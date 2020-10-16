//
// Created by Daniel Bizari on 16/10/2020.
//

#include "logger.h"

#include <utility>

Logger::Logger() = default;

void Logger::setLogLevel(log_level logLevel) {
    this->logLevel = logLevel;
}

void Logger::setPathToFile(std::string pathToFile) {
//    if (pathToFile == "") { //TODO investigar excepciones en C++
//        throw "Error: path to file cannot be empty";
//    }
    this->pathToFile = std::move(pathToFile);
}

log_level Logger::getLogLevel() {
    return this->logLevel;
}

std::string Logger::getPathToFile() {
    return this->pathToFile;
}
