#include "ClientParser.h"

json ClientParser::buildLoginMsg(std::string username, std::string password) {
    return Protocol::buildContentMsg(0, LOGIN_CMD, {
            {MSG_LOGIN_USERNAME, username},
            {MSG_LOGIN_PASSWORD, password}
    });
}

ProtocolCommand ClientParser::getCommand(json msg) {
    return msg[MSG_COMMAND_PROTOCOL].get<ProtocolCommand>();
}

GameMsgParams ClientParser::parseInitParams(json msg) {
    Logger::getInstance()->debug("Parsing init params...");
    json contentJson = msg[MSG_CONTENT_PROTOCOL];
    CameraInit camera = {
            contentJson["camera"]["xPos"],
            contentJson["camera"]["yPos"],
            contentJson["camera"]["width"],
            contentJson["camera"]["height"]
    };

    WindowInit window = {
            contentJson["window"]["width"],
            contentJson["window"]["height"],
    };

    StageInit stage = {
            contentJson["stage"]["level"],
            contentJson["stage"]["timer"],
            contentJson["stage"]["isDefault"],
            contentJson["stage"]["levelLimit"]
    };

    std::vector<GameObjectInit> gameObjects;

    for (auto&gameObjectJson : contentJson["gameObjects"]) {
        GameObjectInit gameObjectInit = {
                gameObjectJson["id"],
                gameObjectJson["type"],
                gameObjectJson["imageId"],
                gameObjectJson["username"],
                gameObjectJson["xPos"],
                gameObjectJson["yPos"],
                gameObjectJson["frameAmount"],
                gameObjectJson["height"],
                gameObjectJson["width"],
                gameObjectJson["hidden"]
        };
        gameObjects.push_back(gameObjectInit);
    }

    return GameMsgParams{
        contentJson["paths"],
        window,
        camera,
        stage,
        { gameObjects },
        contentJson["soundPaths"]
    };
}

GameMsgPlaying ClientParser::parseUpdateParams(json msg) {
    // Logger::getInstance()->debug("Parsing update params...");
    json contentJson = msg[MSG_CONTENT_PROTOCOL];

    CameraDuringGame camera = {
            contentJson["camera"]["xPos"],
            contentJson["camera"]["yPos"]
    };

    std::vector<GamePlayerPlaying> gamePlayers;
    for (auto&player : contentJson["players"]) {
        GamePlayerPlaying gamePlayer = {
                player["id"],
                player["xPos"],
                player["yPos"],
                player["state"],
                player["direction"],
                player["points"],
                player["lives"],
                player["testMode"],
                player["playerBig"]
        };
        gamePlayers.push_back(gamePlayer);
    }

    std::vector<GameObjectPlaying> gameObjects;
    for (auto& go: contentJson["gameObjects"]){
        GameObjectPlaying gameObject = {
                go["id"],
                go["xPos"],
                go["yPos"],
                go["state"],
                go["direction"],
                go["points"],
                go["lives"],
                go["hidden"]
        };
        gameObjects.push_back(gameObject);
    }

    return GameMsgPlaying {
        camera,
        gamePlayers,
        gameObjects,
        contentJson["timer"]
    };
}

GameMsgLevelChange ClientParser::parseChangeLevelParams(json msg) {
    Logger::getInstance()->debug("Parsing change level params...");
    json contentJson = msg[MSG_CONTENT_PROTOCOL];

    StageInit stage = {
            contentJson["stage"]["level"],
            contentJson["stage"]["timer"],
            contentJson["stage"]["isDefault"],
            contentJson["stage"]["levelLimit"]
    };

    std::vector<GameObjectInit> gameObjects;

    for (auto&gameObjectJson : contentJson["gameObjects"]) {
        GameObjectInit gameObjectInit = {
                gameObjectJson["id"],
                gameObjectJson["type"],
                gameObjectJson["imageId"],
                gameObjectJson["username"],
                gameObjectJson["xPos"],
                gameObjectJson["yPos"],
                gameObjectJson["frameAmount"],
                gameObjectJson["height"],
                gameObjectJson["width"],
                gameObjectJson["hidden"]

        };
        gameObjects.push_back(gameObjectInit);
    }

    return GameMsgLevelChange{
            stage,
            { gameObjects }
    };
}

GameMsgShowPartialScore ClientParser::parsePartialScoreParams(json msg) {
    Logger::getInstance()->debug("Parsing partial score params");
    json contentJson = msg[MSG_CONTENT_PROTOCOL];

    std::vector<GameMsgPlayersPartialScore> playersPartialScore;
    for (auto& playerJson: contentJson["playersScore"]) {
        GameMsgPlayersPartialScore player = {
                playerJson["id"],
                playerJson["score"]
        };
        playersPartialScore.push_back(player);
    }

    return GameMsgShowPartialScore {
        contentJson["level"],
        playersPartialScore
    };
}

GameMsgShowGameOver ClientParser::parseGameOverParams(json msg) {
    Logger::getInstance()->debug("Parsing game over params");
    json contentJson = msg[MSG_CONTENT_PROTOCOL];

    std::vector<GameMsgPlayersTotalScore> playersTotal;
    for (auto& playerJson: contentJson["playersScore"]) {
        GameMsgPlayersTotalScore player = {
                playerJson["id"],
                playerJson["levelScores"],
                playerJson["totalScore"],
                playerJson["lives"]
        };
        playersTotal.push_back(player);
    }

    return GameMsgShowGameOver {
        playersTotal,
        contentJson["isTimeOver"]
    };
}

