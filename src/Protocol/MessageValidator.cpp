#include "MessageValidator.h"

std::string MessageValidator::validLoginMessage(json msg) {
    return validMessage(msg, LOGIN_CMD, {MSG_LOGIN_USERNAME, MSG_LOGIN_PASSWORD});
}

std::string MessageValidator::validLoginMessageResponse(json msg) {
    return validMessage(msg, LOGIN_CMD, {MSG_RESPONSE_PROTOCOL});
}

std::string MessageValidator::validMessage(json msg, ProtocolCommand protocol, const std::vector<std::string>& contentTags) {
    if (msg == nullptr) {
        return std::string("No message received");
    }

    if (!msg.contains(MSG_STATUS_PROTOCOL)) {
        return std::string("Status is required");
    }
    int status = msg[MSG_STATUS_PROTOCOL];
    if (status == 1) {
        if (!msg.contains(MSG_ERROR_PROTOCOL)) {
            return std::string("Error is required");
        }
    } else {
        if (!msg.contains(MSG_COMMAND_PROTOCOL)) {
            return std::string("Command is required.");
        }

        ProtocolCommand commandProtocol = msg[MSG_COMMAND_PROTOCOL].get<ProtocolCommand>();
        if (commandProtocol != protocol) {
            return std::string("Message protocol is not expected type " + std::string(json(commandProtocol))); //unchecked
        }

        if (!msg.contains(MSG_CONTENT_PROTOCOL)) {
            return std::string("Content is required.");
        }
        json content = msg[MSG_CONTENT_PROTOCOL];

        for (auto& contentTag: contentTags) {
            if (!content.contains(contentTag)) {
                return std::string("Content tag " + contentTag + " is required");
            }
        }
    }

    return std::string("");
}

