//
// Created by DANIELA CARRERO on 2020-12-05.
//

#ifndef TPTALLER1_MESSAGEVALIDATOR_H
#define TPTALLER1_MESSAGEVALIDATOR_H

#include <json.hpp>
#include "Protocol.h"

using json = nlohmann::json;
class MessageValidator {
public:
    static std::string validLoginMessage(json msg);

private:
    ~MessageValidator() = default;
    MessageValidator() = default;
};


#endif //TPTALLER1_MESSAGEVALIDATOR_H
