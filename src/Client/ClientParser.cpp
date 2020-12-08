#include "ClientParser.h"

json ClientParser::buildLoginMsg(std::string username, std::string password) {
    return Protocol::buildContentMsg(0, LOGIN_CMD, {
            {MSG_LOGIN_USERNAME, username},
            {MSG_LOGIN_PASSWORD, password}
    });
}