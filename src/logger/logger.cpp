//
// Created by Daniel Bizari on 16/10/2020.
//

#include "logger.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "loggerException.h"

Logger* Logger::instance = nullptr; //Por alguna razon si no pongo esto explota

Logger::Logger() {
    std::string now = getTimestamp();
    std::string path = "./log_" + now.substr(11, 8) + ".txt";
    this->myFile.open(path,std::fstream::in | std::fstream::out | std::fstream::app);
    if(!myFile.is_open()) {
        std::cerr << "Fatal error: unable to create log file" << std::endl;
        throw loggerException("unable to create log file");
    }

    this->logLevel = DEBUG; //Default level
};

Logger *Logger::getInstance() {
    if (Logger::instance == nullptr) {
        Logger::instance = new Logger();
    }

    return instance;
}

std::string Logger::getTimestamp() {
    std::time_t now = std::time(nullptr);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S",  std::localtime(&now));
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) % 1000;
    std::ostringstream oss;

    oss << timestamp << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}

void Logger::setLogLevel(std::string logLevelInput) {
    if(logLevelInput == "ERROR") {
        this->logLevel = ERROR;
    } else if (logLevelInput == "INFO") {
        this->logLevel = INFO;
    } else if (logLevelInput == "DEBUG") {
        this->logLevel = DEBUG;
    } else {
        std::cerr << "wrong log level" << std::endl;
        throw loggerException("inexistent log level");
    }
}

log_level Logger::getLogLevel() {
    return this->logLevel;
}

Logger::~Logger() {
    this->myFile.flush();
    this->myFile.close();
}

std::string Logger::info(std::string msg) {
    if(this->logLevel >= INFO) {
        return writeMsg(msg, "INFO");
    }
    return "";
}

std::string Logger::debug(std::string msg) {
    if(this->logLevel >= DEBUG) {
        return writeMsg(msg, "DEBUG");
    }
    return "";
}

std::string Logger::error(std::string msg) {
    return writeMsg(msg, "ERROR");
}

std::string Logger::writeMsg(std::string msg, std::string logLevel) {
    std::ostringstream logMsg;
    if(this->myFile.is_open()) {
        this->myFile << getTimestamp() << " [" << logLevel << "]: " << msg << std::endl;
        logMsg << getTimestamp() << " [" << logLevel << "]: " << msg << std::endl;
    } else {
        std::cerr << "FATAL error: cannot write in closed file" << std::endl;
        throw loggerException("cannot write in closed file");
    }

    return logMsg.str();
};
