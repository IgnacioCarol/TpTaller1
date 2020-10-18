//
// Created by Daniel Bizari on 18/10/2020.
//

#include "logger_exception.h"

const char *logger_exception::what() const _NOEXCEPT {
    return this->msg.c_str();
}

logger_exception::logger_exception(std::string msg) {
    this->msg = msg;
}
