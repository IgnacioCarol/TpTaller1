#ifndef TPTALLER1_PROTOCOL_H
#define TPTALLER1_PROTOCOL_H

#include <iostream>
#include <json.hpp>
#include "GameMsgParams.h"

using json = nlohmann::json;

#define MSG_ERROR_PROTOCOL "error"
#define MSG_STATUS_PROTOCOL "status"
#define MSG_COMMAND_PROTOCOL "command"
#define MSG_CONTENT_PROTOCOL "content"
#define MSG_RESPONSE_PROTOCOL "response"

#define MSG_LOGIN_AUTHORIZED "AUTHORIZED"
#define MSG_LOGIN_UNAUTHORIZED "UNAUTHORIZED"

#define MSG_LOGIN_USERNAME "username"
#define MSG_LOGIN_PASSWORD "password"

typedef enum {
    LOGIN_CMD,
    GAME_INITIALIZE_CMD,
    GAME_VIEW_CMD
} ProtocolCommand;

NLOHMANN_JSON_SERIALIZE_ENUM(ProtocolCommand, {
    { LOGIN_CMD, "login" },
    { GAME_INITIALIZE_CMD, "game_initialize" },
    { GAME_VIEW_CMD, "game_view" }
})

class Protocol {
public:
    static std::string protocolToString(ProtocolCommand command);
    static json buildErrorMsg(std::string error);
    static json buildContentMsg(int status, ProtocolCommand commnad, json content);

    static json gameInitMsgToJson(GameMsgParams params);

private:
    Protocol() = default;
    ~Protocol() = default;
};

#endif //TPTALLER1_PROTOCOL_H
