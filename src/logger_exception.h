//
// Created by Daniel Bizari on 18/10/2020.
//

#ifndef TPTALLER1_LOGGER_EXCEPTION_H
#define TPTALLER1_LOGGER_EXCEPTION_H

#include <exception>
#include <iostream>

class logger_exception : std::exception {
    public:
        const char* what() const _NOEXCEPT override;
        explicit logger_exception(std::string msg);

    private:
        std::string msg;
};


#endif //TPTALLER1_LOGGER_EXCEPTION_H
