#include "ServerParser.h"

json ServerParser::buildLoginMsgResponse(bool authenticated) {
    return Protocol::buildContentMsg(0, LOGIN_CMD, {
            {MSG_RESPONSE_PROTOCOL, (authenticated ? MSG_LOGIN_AUTHORIZED : MSG_LOGIN_UNAUTHORIZED) }
    });
}

json ServerParser::buildErrorMsg(std::string error) {
    return Protocol::buildErrorMsg(error);
}

json ServerParser::buildGameInitMsg() {
    return json();
}
