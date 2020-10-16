//
// Created by DANIELA CARRERO on 2020-10-15.
//

#ifndef CONFIG_H
#define CONFIG_H

#define DEFAULT_LOG_LEVEL "ERROR"
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_STAGE_LEVEL_NUMBER 1
#define DEFAULT_STAGE_LEVEL_BACKGROUND "/home/file21.bmp" //TODO: Change file with the correct one
#define DEFAULT_STAGE_LEVEL_COINS 30
#define DEFAULT_STAGE_LEVEL_ENEMY_QTY 20
#define DEFAULT_STAGE_LEVEL_ENEMY_TYPE 1
#define DEFAULT_STAGE_LEVEL_ENEMY_IMG "/home/file3.bmp" //TODO: Change file with the correct one
#define DEFAULT_PLATFORM_TYPE "brick"
#define DEFAULT_PLATFORM_COORD_X 100
#define DEFAULT_PLATFORM_COORD_Y 200
#define DEFAULT_PLATFORM_QTY 5

#include <list>
#include <string>
using namespace std;

struct Enemy {
    int type = DEFAULT_STAGE_LEVEL_ENEMY_TYPE;
    string image = DEFAULT_STAGE_LEVEL_ENEMY_IMG;
    int quantity = DEFAULT_STAGE_LEVEL_ENEMY_QTY;
};

struct Platform {
    string type = DEFAULT_PLATFORM_TYPE;
    int coordX = DEFAULT_PLATFORM_COORD_X;
    int coordY = DEFAULT_PLATFORM_COORD_Y;
    int quantity = DEFAULT_PLATFORM_QTY;
};

struct Level {
    Level() {
        Enemy enemy;
        Platform platform;

        enemies.push_front(enemy);
        platforms.push_front(platform);
    }

    int number = DEFAULT_STAGE_LEVEL_NUMBER;
    string background = DEFAULT_STAGE_LEVEL_BACKGROUND;
    int coins = DEFAULT_STAGE_LEVEL_COINS;
    list<Enemy> enemies;
    list<Platform> platforms;
};

struct Stage {
    Stage() {
        Level level;

        levels.push_front(level);
    }

    list<Level> levels;
};

struct Window {
    int width = DEFAULT_WINDOW_WIDTH;
    int height = DEFAULT_WINDOW_HEIGHT;
};

struct Log {
    string level = DEFAULT_LOG_LEVEL;
};

class Config {
public:
    Config();
    ~Config();

    Window getWindow();
    Stage getStage();
    void setWindow();
    void setStage();

    void load(const std::string &filename);
private:
    Window window;
    Stage stage;
};


#endif //CONFIG_H
