//
// Created by DANIELA CARRERO on 2020-11-28.
//

#include "ServerException.h"

const char* ServerException::what() const noexcept {
    return this->msg.c_str();
}

ServerException::ServerException(std::string msg) {
    this->msg = msg;
}