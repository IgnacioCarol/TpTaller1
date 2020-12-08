#ifndef TPTALLER1_SERVERPARSER_H
#define TPTALLER1_SERVERPARSER_H

#include "json.hpp"
#include "../Protocol/Protocol.h"

using json = nlohmann::json;

class ServerParser {
public:
    static json buildLoginMsgResponse(bool authenticated);
    static json buildGameInitMsg();
    static json buildErrorMsg(std::string error);
};


#endif //TPTALLER1_SERVERPARSER_H
