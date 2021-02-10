#ifndef TPTALLER1_CLIENTPARSER_H
#define TPTALLER1_CLIENTPARSER_H

#include "json.hpp"
#include "../Protocol/Protocol.h"
#include "../logger/logger.h"

using json = nlohmann::json;

class ClientParser {
public:
    static json buildLoginMsg(std::string username, std::string password);
    static ProtocolCommand getCommand(json msg);
    static GameMsgParams parseInitParams(json json);
    static GameMsgPlaying parseUpdateParams(json json);
    static GameMsgLevelChange parseChangeLevelParams(json json);

    static GameMsgShowPartialScore parsePartialScoreParams(json json);
    static GameMsgShowGameOver parseGameOverParams(json json);
};


#endif //TPTALLER1_CLIENTPARSER_H
