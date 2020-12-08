#include "ServerParser.h"

json ServerParser::buildLoginMsgResponse(bool authenticated) {
    return Protocol::buildContentMsg(0, LOGIN_CMD, {
            {MSG_RESPONSE_PROTOCOL, (authenticated ? MSG_LOGIN_AUTHORIZED : MSG_LOGIN_UNAUTHORIZED) }
    });
}

json ServerParser::buildErrorMsg(std::string error) {
    return Protocol::buildErrorMsg(error);
}

json ServerParser::buildGameInitMsg(std::map<std::string, std::vector<std::string>> imagePaths, Camera *camera,
        BackgroundStage *stage, std::vector<GameObject *> gameObjects, std::vector<Player *> players) {
    CameraInit cameraInit = {
            camera->getCamera()->x,
            camera->getCamera()->y,
            camera->getCamera()->w,
            camera->getCamera()->h
    };

    StageInit stageInit = {
            stage->getLevel(),
            stage->getTimer()->getTimeSecond(),
            Config::getInstance()->isDefault()
    };

    WindowInit windowInit = {
            Config::getInstance()->getWindow().width,
            Config::getInstance()->getWindow().height
    };

    std::vector<GameObjectInit> gameObjectsInit;
    for (auto& gameObject: gameObjects) {
        GameObjectInit gameObjectInit = {
                gameObject->getId(),
                gameObject->getType(),
                gameObject->getTextureId(),
                "",
                gameObject->getXPosition(),
                gameObject->getYPosition(),
                gameObject->getFrameAmount()
        };
        gameObjectsInit.push_back(gameObjectInit);
    }

    for (auto& player: players) {
        GameObjectInit gameObjectInit = {
                player->getId(),
                player->getType(),
                player->getTextureId(),
                player->getUsername(),
                player->getXPosition(),
                player->getYPosition(),
                player->getFrameAmount()
        };
        gameObjectsInit.push_back(gameObjectInit);
    }

    GameMsgParams gameMsgParams = {
        imagePaths,
        windowInit,
        cameraInit,
        stageInit,
        gameObjectsInit
    };

    return Protocol::gameMsgToJson(gameMsgParams);
}
