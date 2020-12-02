//
// Created by Daniel Bizari on 23/11/2020.
//

#include "SocketException.h"

const char *SocketException::what() const noexcept {
    return this->msg.c_str();
}

SocketException::SocketException(std::string msg) {
    this->msg = msg;
}