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
    static json buildGameInitMsg(Camera *camera, BackgroundStage *stage, std::vector<GameObject *> gameObjects);
    static json buildErrorMsg(std::string error);
};


#endif //TPTALLER1_SERVERPARSER_H
