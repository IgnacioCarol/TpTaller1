#ifndef TPTALLER1_CLIENTPARSER_H
#define TPTALLER1_CLIENTPARSER_H

#include "json.hpp"
#include "../Protocol/Protocol.h"

using json = nlohmann::json;

class ClientParser {
public:
    static json buildLoginMsg(std::string username, std::string password);
    static ProtocolCommand getCommand(json msg);
};


#endif //TPTALLER1_CLIENTPARSER_H
