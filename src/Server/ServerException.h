#ifndef TPTALLER1_SERVEREXCEPTION_H
#define TPTALLER1_SERVEREXCEPTION_H

#include <exception>
#include <iostream>

class ServerException : public std::exception {
public:
    const char* what() const noexcept override;
    explicit ServerException(std::string msg);

private:
    std::string msg;
};


#endif //TPTALLER1_SERVEREXCEPTION_H
