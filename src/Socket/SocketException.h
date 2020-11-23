//
// Created by Daniel Bizari on 23/11/2020.
//

#ifndef TPTALLER1_SOCKETEXCEPTION_H
#define TPTALLER1_SOCKETEXCEPTION_H

#include <exception>
#include <iostream>

class SocketException : public std::exception {
public:
    const char* what() const noexcept override;
    explicit SocketException(std::string msg);

private:
    std::string msg;
};

#endif //TPTALLER1_SOCKETEXCEPTION_H
