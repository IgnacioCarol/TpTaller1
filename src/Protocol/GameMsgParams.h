#ifndef TPTALLER1_GAMEMSGPARAMS_H
#define TPTALLER1_GAMEMSGPARAMS_H

#include <iostream>
#include <vector>
#include <map>
#include "json.hpp"
#include "../gameobjects/GameObject.h"

NLOHMANN_JSON_SERIALIZE_ENUM(GameObjectType, {
    { GOT_COIN, "coin" },
    { GOT_ENEMY_MUSHROOM, "mushroom" },
    { GOT_ENEMY_TURTLE, "turtle" },
    { GOT_PLATFORM_NORMAL, "normal_platform" },
    { GOT_PLATFORM_SURPRISE, "surprise_platform" },
    { GOT_PLAYER, "player" },
    { GOT_HOLE, "hole" },
    { GOT_PIPE, "pipe" },
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
    int levelLimit;
};

struct GameObjectInit {
    int id;
    GameObjectType type;
    std::string imageId;
    std::string username;
    int xPos;
    int yPos;
    int frameAmount;
    int height;
    int width;
};

struct GameObjectsInit {
    std::vector<GameObjectInit> gameObjects;
};

struct GameMsgParams {
    std::map<std::string, std::vector<std::string>> paths;
    WindowInit window;
    CameraInit camera;
    StageInit stage;
    GameObjectsInit gameObjectsInit;
    std::map<std::string, std::string> soundPaths;
};

struct GameMsgLevelChange {
    StageInit stage;
    GameObjectsInit gameObjectsInit;
};

struct GamePlayerPlaying {
    int id;
    int xPos;
    int yPos;
    std::string state;
    bool direction;
    bool testMode;
    int lives;
};

struct GameObjectPlaying {
    int id;
    int xPos;
    int yPos;
    std::string state;
    bool direction;
};

struct CameraDuringGame {
    int xPos;
    int yPos;
};

struct GameMsgPlaying {
    CameraDuringGame camera;
    std::vector<GamePlayerPlaying> players;
    std::vector<GameObjectPlaying> gameObjects;
    int timer;
};
#endif //TPTALLER1_GAMEMSGPARAMS_H
