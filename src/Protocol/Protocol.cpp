#include "Protocol.h"

//TODO: borrar si funciona bien el enum conversion de nlohmann
std::string Protocol::protocolToString(ProtocolCommand protocol) {
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

json Protocol::buildErrorMsg(std::string error) {
    return {
            {MSG_STATUS_PROTOCOL, 1},
            {MSG_ERROR_PROTOCOL, error}
    };
}

json Protocol::buildContentMsg(int status, ProtocolCommand command, json content) {
    return {
            {MSG_STATUS_PROTOCOL, status},
            {MSG_COMMAND_PROTOCOL, command},
            {MSG_CONTENT_PROTOCOL, content}
    };
}

