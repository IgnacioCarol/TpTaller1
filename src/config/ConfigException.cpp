//
// Created by DANIELA CARRERO on 2020-10-31.
//

#include "ConfigException.h"


const char *ConfigException::what() const noexcept {
    return this->msg.c_str();
}

ConfigException::ConfigException(std::string msg) {
    this->msg = msg;
}