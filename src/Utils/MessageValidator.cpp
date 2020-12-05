#include "MessageValidator.h"

std::string MessageValidator::validLoginMessage(json msg) {
    if (msg == nullptr) {
        return std::string("No message received");
    }

    if (msg[MSG_COMMAND_PROTOCOL] != Protocol::protocolToString(LOGIN_CMD)) {
        return std::string("Message is not login type");
    }

    json content = msg[MSG_CONTENT_PROTOCOL];

    if (content == nullptr || content[MSG_LOGIN_USERNAME] == nullptr || content[MSG_LOGIN_PASSWORD] == nullptr) {
        return std::string("Unexpected content. Username and password are required.");
    }

    return std::string("");
}

