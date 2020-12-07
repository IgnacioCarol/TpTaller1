#ifndef TPTALLER1_INITIALIZEGAMEMSG_H
#define TPTALLER1_INITIALIZEGAMEMSG_H

#include <iostream>
#include <vector>
#include <map>

enum ObjectType {
    COIN,
    MUSHROOM,
    TURTLE,
    NORMAL_PLATFORM,
    SURPRISE_PLATFORM,
    PLAYER
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

struct GameObjectsInit {
    std::vector<GameObjectInit> gameObjects;
};

struct InitializeGameMsg {
    std::map<std::string, std::vector<std::string>> paths;
    CameraInit camera;
    StageInit stage;
    GameObjectsInit gameObjects;
};

#endif //TPTALLER1_INITIALIZEGAMEMSG_H
