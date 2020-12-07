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
    static std::string validLoginMessageResponse(json msg);

private:
    ~MessageValidator() = default;
    MessageValidator() = default;

    static std::string validMessage(json msg, PROTOCOL_COMMAND protocol, const std::vector<std::string>& contentTags);
};


#endif //TPTALLER1_MESSAGEVALIDATOR_H
