#include "ClientException.h"

const char* ClientException::what() const noexcept {
    return this->msg.c_str();
}

ClientException::ClientException(std::string msg) {
    this->msg = msg;
}