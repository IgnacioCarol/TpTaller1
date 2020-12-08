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

json Protocol::gameMsgToJson(GameMsgParams params) {
    json window = {
            {"width", params.window.width},
            {"height", params.window.height}
    };

    json camera = {
            {"xPos", params.camera.xPos},
            {"yPos", params.camera.yPos},
            {"width", params.camera.width},
            {"height", params.camera.height}
    };

    json stage = {
            {"level", params.stage.level},
            {"timer", params.stage.timer},
            {"isDefault", params.stage.isDefault}
    };

    json gameObjects = json::array();
    for (auto& gameObject : params.gameObjects) {
        json gameObjectJson = {
                gameObject.id,
                gameObject.type,
                gameObject.imageId,
                gameObject.username,
                gameObject.xPos,
                gameObject.yPos,
                gameObject.frameAmount
        };
        gameObjects.push_back(gameObjectJson);
    }

    json msg = {
            {"camera",      camera},
            {"window",      window},
            {"stage",       stage},
            {"gameObjects", gameObjects}
    };

    return msg;
}

