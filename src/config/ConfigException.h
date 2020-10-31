//
// Created by DANIELA CARRERO on 2020-10-31.
//

#ifndef TPTALLER1_CONFIGEXCEPTION_H
#define TPTALLER1_CONFIGEXCEPTION_H

#include <exception>
#include <iostream>

class ConfigException : public std::exception {
public:
    const char* what() const noexcept override;
    explicit ConfigException(std::string msg);

private:
    std::string msg;
};


#endif //TPTALLER1_CONFIGEXCEPTION_H
