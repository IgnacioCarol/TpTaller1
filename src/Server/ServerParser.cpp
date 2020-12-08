#include "ServerParser.h"

json ServerParser::buildLoginMsgResponse(bool authenticated) {
    return Protocol::buildContentMsg(0, LOGIN_CMD, {
            {MSG_RESPONSE_PROTOCOL, (authenticated ? MSG_LOGIN_AUTHORIZED : MSG_LOGIN_UNAUTHORIZED) }
    });
}

json ServerParser::buildErrorMsg(std::string error) {
    return Protocol::buildErrorMsg(error);
}

json ServerParser::buildGameInitMsg(Camera *camera, BackgroundStage *stage, std::vector<GameObject *> gameObjects) {
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



    return json();
}
