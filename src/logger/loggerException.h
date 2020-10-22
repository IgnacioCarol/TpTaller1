//
// Created by Daniel Bizari on 18/10/2020.
//

#ifndef TPTALLER1_LOGGEREXCEPTION_H
#define TPTALLER1_LOGGEREXCEPTION_H

#include <exception>
#include <iostream>

class loggerException : public std::exception {
    public:
        //const char* what() const _NOEXCEPT override;
        explicit loggerException(std::string msg);

    private:
        std::string msg;
};


#endif //TPTALLER1_LOGGEREXCEPTION_H
