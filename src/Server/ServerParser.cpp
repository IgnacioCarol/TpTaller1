#include "ServerParser.h"

json ServerParser::buildLoginMsgResponse(bool authenticated) {
    return Protocol::buildContentMsg(0, LOGIN_CMD, {
            {MSG_RESPONSE_PROTOCOL, (authenticated ? MSG_LOGIN_AUTHORIZED : MSG_LOGIN_UNAUTHORIZED) }
    });
}

json ServerParser::buildErrorMsg(std::string error) {
    return Protocol::buildErrorMsg(error);
}

json ServerParser::buildGameInitMsg(std::map<std::string, std::vector<std::string>> imagePaths,
                                    std::map<std::string, std::string> soundPaths, Camera *camera,
                                    BackgroundStage *stage,
                                    std::vector<GameObject *> gameObjects, std::vector<Player *> players) {
    CameraInit cameraInit = {
            camera->getCamera()->x,
            camera->getCamera()->y,
            camera->getCamera()->w,
            camera->getCamera()->h
    };

    StageInit stageInit = {
            stage->getLevel(),
            stage->getTimer()->getTimeSecond(),
            Config::getInstance()->isDefault(),
            stage->getLevelLimit()
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
        gameObjectsInit,
        soundPaths
    };

    return Protocol::gameInitMsgToJson(gameMsgParams);
}
json ServerParser::buildPlayingGameMessage(std::vector<Player *> players, std::vector<GameObject *> objects,
                                           Camera *camera,
                                           int timer) {
    CameraDuringGame cameraDuringGame = {
            camera->getCamera()->x,
            camera->getCamera()->y,
    };
    std::vector<GamePlayerPlaying> gamePlayers;
    for (Player* player: players) {
        GamePlayerPlaying gamePlayer = {
                player->getId(),
                player->getXPosition(),
                player->getYPosition(),
                player->getState(),
                player->getDirection(),
                player->getTestModeState()
        };
        gamePlayers.push_back(gamePlayer);
    }

    std::vector<GameObjectPlaying> gameObjects;
    for (GameObject* go: objects){
        if (!go->isAtScene(camera->getXpos())) continue;
        GameObjectPlaying object = {
                go->getId(),
                go->getXPosition(),
                go->getYPosition(),
                go->getState(),
                go->getDirection()
        };
        gameObjects.push_back(object);
    }

    GameMsgPlaying gameMsgPlaying = {
            cameraDuringGame,
            gamePlayers,
            gameObjects,
            timer

    };
    return Protocol::gameViewMsgToJson(gameMsgPlaying);
}

json ServerParser::buildGameOverMsg() {
    return Protocol::buildContentMsg(0, GAME_OVER_CMD, {});
}

json ServerParser::buildChangeLevelMsg(std::vector<GameObject *> gameObjects, BackgroundStage *stage) {
    StageInit stageUpdate = {
            stage->getLevel(),
            stage->getTimer()->getTimeSecond(),
            Config::getInstance()->isDefault(),
            stage->getLevelLimit()
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

    GameMsgLevelChange gameMsgLevelChange = {
            stageUpdate,
            gameObjectsInit
    };

    return Protocol::gameChangeLevelMsgToJson(gameMsgLevelChange);
}
