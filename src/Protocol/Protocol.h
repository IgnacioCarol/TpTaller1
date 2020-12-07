#ifndef TPTALLER1_PROTOCOL_H
#define TPTALLER1_PROTOCOL_H

#include <iostream>
#include <json.hpp>
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
} PROTOCOL_COMMAND;

class Protocol {
public:
    static std::string protocolToString(PROTOCOL_COMMAND);
    static json buildErrorMsg(std::string error);
    static json buildLoginMsgResponse(bool authenticated);
    static json buildLoginMsg(std::string username, std::string password);

private:
    Protocol() = default;
    ~Protocol() = default;
};

#endif //TPTALLER1_PROTOCOL_H
