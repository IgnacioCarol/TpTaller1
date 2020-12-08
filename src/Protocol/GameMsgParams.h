#ifndef TPTALLER1_GAMEMSGPARAMS_H
#define TPTALLER1_GAMEMSGPARAMS_H

#include <iostream>
#include <vector>
#include <map>
#include "json.hpp"

enum ObjectType {
    OT_COIN,
    OT_MUSHROOM,
    OT_TURTLE,
    OT_NORMAL_PLATFORM,
    OT_SURPRISE_PLATFORM,
    OT_PLAYER
};

NLOHMANN_JSON_SERIALIZE_ENUM(ObjectType, {
    { OT_COIN, "coin" },
    { OT_MUSHROOM, "mushroom" },
    { OT_TURTLE, "turtle" },
    { OT_NORMAL_PLATFORM, "normal_platform" },
    { OT_SURPRISE_PLATFORM, "surprise_platform" },
    { OT_PLAYER, "player" },
})

struct WindowInit {
    int width;
    int height;
};

struct CameraInit {
    int xPos;
    int yPos; // Puede que no haga falta
    int width;
    int height;
};

struct StageInit {
    int level;
    int timer;
    bool isDefault;
};

struct GameObjectInit {
    int id;
    ObjectType type;
    std::string imageId;
    int xPos;
    int yPos;
    int frameAmount;
};

struct PlayerInit : GameObjectInit {
    std::string username;
    std::string idImage;
};

struct GameMsgParams {
    std::map<std::string, std::vector<std::string>> paths;
    WindowInit window;
    CameraInit camera;
    StageInit stage;
    std::vector<GameObjectInit> gameObjects;
};

#endif //TPTALLER1_GAMEMSGPARAMS_H
