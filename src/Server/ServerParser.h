#ifndef TPTALLER1_SERVERPARSER_H
#define TPTALLER1_SERVERPARSER_H

#include "json.hpp"
#include "../Protocol/Protocol.h"
#include "../Camera.h"
#include "../BackgroundStages/BackgroundStage.h"
#include "../Protocol/GameMsgParams.h"

using json = nlohmann::json;
class Camera;
class ServerParser {
public:
    static json buildLoginMsgResponse(bool authenticated);
    static json buildGameInitMsg(std::map<std::string, std::vector<std::string>> imagePaths,
                                 std::map<std::string, std::string> soundPaths, Camera *camera, BackgroundStage *stage,
                                 std::vector<GameObject *> gameObjects, std::vector<Player *> players);
    static json buildErrorMsg(std::string error);

    static json
    buildPlayingGameMessage(std::vector<Player *> players, std::vector<GameObject *> gameObjects, Camera *camera,
                            int timer);

    static json buildChangeLevelMsg(std::vector<GameObject*> gameObjects, BackgroundStage* stage);

    static json buildGameOverMsg(std::vector<Player*> players);

    static json buildPartialScore(std::vector<Player*> players, BackgroundStage *stage);

    static json buildStopPartialScore();
};


#endif //TPTALLER1_SERVERPARSER_H
