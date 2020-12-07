#ifndef TPTALLER1_INITIALIZEGAMEMSG_H
#define TPTALLER1_INITIALIZEGAMEMSG_H

#include <iostream>
#include <vector>

struct CameraInit {
    int xPos;
    int yPos;
    int width;
    int height;
};

struct StageInit {
    std::string path;
    int level;
    int timer;
    int width;
};

struct GameObjectInit {
    int id;
    std::string type;
    std::string path;
    std::string defaultPath;
    int xPos;
    int yPos;
};

struct PlayerInit : GameObjectInit {
    std::string username;
};

struct GameObjectsInit {
    std::vector<GameObjectInit> gameObjects;
};

struct InitializeGameMsg {
    CameraInit camera;
    StageInit stage;
    GameObjectsInit gameObjects;
};

#endif //TPTALLER1_INITIALIZEGAMEMSG_H
