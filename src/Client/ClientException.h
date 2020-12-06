#ifndef TPTALLER1_CLIENTEXCEPTION_H
#define TPTALLER1_CLIENTEXCEPTION_H

#include <exception>
#include <iostream>

class ClientException : public std::exception {
public:
    const char* what() const noexcept override;
    explicit ClientException(std::string msg);

private:
    std::string msg;
};

#endif //TPTALLER1_CLIENTEXCEPTION_H
