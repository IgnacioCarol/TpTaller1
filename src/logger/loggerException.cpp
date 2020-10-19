//
// Created by Daniel Bizari on 18/10/2020.
//

#include "loggerException.h"

const char *loggerException::what() const _NOEXCEPT {
    return this->msg.c_str();
}

loggerException::loggerException(std::string msg) {
    this->msg = msg;
}
