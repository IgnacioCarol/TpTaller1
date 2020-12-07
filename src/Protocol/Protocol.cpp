#include "Protocol.h"

std::string Protocol::protocolToString(PROTOCOL_COMMAND protocol) {
    switch (protocol) {
        case LOGIN_CMD:
            return std::string("login");
        case GAME_INITIALIZE_CMD:
            return std::string("game_initialize");
        case GAME_VIEW_CMD:
            return std::string("game_view");
        default:
            return "";
    }
}

json Protocol::buildLoginMsgResponse(bool authenticated) {
    return {
            {MSG_STATUS_PROTOCOL, 0},
            {MSG_COMMAND_PROTOCOL, protocolToString(LOGIN_CMD)},
            {MSG_CONTENT_PROTOCOL,{
                    {MSG_RESPONSE_PROTOCOL, (authenticated ? MSG_LOGIN_AUTHORIZED : MSG_LOGIN_UNAUTHORIZED) }
                }
            }
    };
}

json Protocol::buildErrorMsg(std::string error) {
    return {
            {MSG_STATUS_PROTOCOL, 1},
            {MSG_ERROR_PROTOCOL, error}
    };
}

json Protocol::buildLoginMsg(std::string username, std::string password) {
    return {
            {MSG_STATUS_PROTOCOL, 0},
            {MSG_COMMAND_PROTOCOL, protocolToString(LOGIN_CMD)},
            {MSG_CONTENT_PROTOCOL, {
                    {MSG_LOGIN_USERNAME, username},
                    {MSG_LOGIN_PASSWORD, password}
                }
            }
    };
}

