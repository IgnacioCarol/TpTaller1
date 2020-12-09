#ifndef TPTALLER1_SERVERPARSER_H
#define TPTALLER1_SERVERPARSER_H

#include "json.hpp"
#include "../Protocol/Protocol.h"
#include "../Camera.h"
#include "../BackgroundStages/BackgroundStage.h"
#include "../Protocol/GameMsgParams.h"

using json = nlohmann::json;

class ServerParser {
public:
    static json buildLoginMsgResponse(bool authenticated);
    static json buildGameInitMsg(std::map<std::string, std::vector<std::string>> imagePaths, Camera *camera,
            BackgroundStage *stage, std::vector<GameObject *> gameObjects, std::vector<Player *> players);
    static json buildErrorMsg(std::string error);

    static json buildPlayingGameMessage(std::vector<Player *> players, Camera *camera, int timer);

    static json buildChangeLevelMsg(std::vector<GameObject*> gameObjects, BackgroundStage* stage);

    static json buildGameOverMsg();
};


#endif //TPTALLER1_SERVERPARSER_H
