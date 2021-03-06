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

json Protocol::gameInitMsgToJson(GameMsgParams params) {
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
            {"isDefault", params.stage.isDefault},
            {"levelLimit", params.stage.levelLimit}
    };

    json gameObjects = json::array();
    for (auto& gameObject : params.gameObjectsInit.gameObjects) {
        json gameObjectJson = {
                {"id", gameObject.id},
                {"type", gameObject.type},
                {"imageId", gameObject.imageId},
                {"username", gameObject.username},
                {"xPos", gameObject.xPos},
                {"yPos", gameObject.yPos},
                {"frameAmount", gameObject.frameAmount},
                {"height", gameObject.height},
                {"width", gameObject.width},
                {"hidden", gameObject.hidden}
        };
        gameObjects.push_back(gameObjectJson);
    }

    json msg = {
            {"paths", params.paths},
            {"camera",      camera},
            {"window",      window},
            {"stage",       stage},
            {"gameObjects", gameObjects},
            {"soundPaths", params.soundPaths}
    };

    return buildContentMsg(0, GAME_INITIALIZE_CMD, msg);
}

json Protocol::gameViewMsgToJson(GameMsgPlaying params) {
    json camera = {
            {"xPos", params.camera.xPos},
            {"yPos", params.camera.yPos},
    };
    json players = json::array();
    for (auto& gameObject : params.players) {
        json gameObjectJson = {
                {"id", gameObject.id},
                {"xPos", gameObject.xPos},
                {"yPos", gameObject.yPos},
                {"state", gameObject.state},
                {"direction", gameObject.direction},
                {"points", gameObject.points},
                {"lives", gameObject.lives},
                {"testMode", gameObject.testMode},
                {"playerBig", gameObject.playerBig}
        };
        players.push_back(gameObjectJson);
    }

    json gameObjects = json::array();
    for (auto& gameObject : params.gameObjects) {
        json gameObjectJson = {
                {"id", gameObject.id},
                {"xPos", gameObject.xPos},
                {"yPos", gameObject.yPos},
                {"state", gameObject.state},
                {"direction", gameObject.direction},
                {"points", gameObject.points},
                {"lives", gameObject.lives},
                {"hidden", gameObject.hidden}
        };
        gameObjects.push_back(gameObjectJson);
    }

    json msg = {
            {"camera", camera},
            {"players", players},
            {"gameObjects", gameObjects},
            {"timer", params.timer}
    };
    return buildContentMsg(0, GAME_VIEW_CMD, msg);
}

json Protocol::gameChangeLevelMsgToJson(GameMsgLevelChange params) {
    json stage = {
            {"level", params.stage.level},
            {"timer", params.stage.timer},
            {"isDefault", params.stage.isDefault},
            {"levelLimit", params.stage.levelLimit}
    };

    json gameObjects = json::array();
    for (auto& gameObject : params.gameObjectsInit.gameObjects) {
        json gameObjectJson = {
                {"id", gameObject.id},
                {"type", gameObject.type},
                {"imageId", gameObject.imageId},
                {"username", gameObject.username},
                {"xPos", gameObject.xPos},
                {"yPos", gameObject.yPos},
                {"frameAmount", gameObject.frameAmount},
                {"height", gameObject.height},
                {"width", gameObject.width},
                {"hidden", gameObject.hidden}
        };
        gameObjects.push_back(gameObjectJson);
    }

    json msg = {
            {"stage",       stage},
            {"gameObjects", gameObjects}
    };

    return buildContentMsg(0, GAME_CHANGE_LEVEL_CMD, msg);
}

json Protocol::gameShowPartialScoreMsgToJson(GameMsgShowPartialScore params) {
    json playersScore = json::array();
    for (auto& player : params.playersPartialScore) {
        json playerPartialScoreJson = {
                {"id", player.id},
                {"score", player.score}
        };
        playersScore.push_back(playerPartialScoreJson);
    }

    json msg = {
            {"level", params.level},
            {"playersScore", playersScore}
    };

    return buildContentMsg(0, GAME_SHOW_PARTIAL_SCORE_CMD, msg);
}

json Protocol::gameShowGameOverMsgToJson(GameMsgShowGameOver params) {
    json playersScore = json::array();

    for (auto& player : params.playersTotalScore) {
        json playerTotalScoreJson = {
                {"id", player.id},
                {"levelScores", player.levelScores},
                {"totalScore", player.totalScore},
                {"lives", player.lives}
        };
        playersScore.push_back(playerTotalScoreJson);
    }

    json msg = {
            {"playersScore", playersScore},
            {"isTimeOver", params.isTimeOver}
    };

    return buildContentMsg(0, GAME_OVER_CMD, msg);
}
