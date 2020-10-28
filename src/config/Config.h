//
// Created by DANIELA CARRERO on 2020-10-15.
//

#ifndef CONFIG_H
#define CONFIG_H

#define XML_STAGE_LEVEL_ENEMY_TURTLE   "enemigo_tortuga"
#define XML_STAGE_LEVEL_ENEMY_MUSHROOM "enemigo_hongo"
typedef enum{ENEMY_MUSHROOM, ENEMY_TURTLE} enemyType;


#define XML_STAGE_LEVEL_PLATFORM_NORMAL   "plataforma_normal"
#define XML_STAGE_LEVEL_PLATFORM_SURPRISE "plataforma_sorpresa"
typedef enum{PLATFORM_NORMAL, PLATFORM_SURPRISE} platformType;

#define DEFAULT_LOG_LEVEL "ERROR"
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_STAGE_LEVEL_NUMBER 1
#define DEFAULT_STAGE_LEVEL_BACKGROUND "/home/file21.bmp" //TODO: Change file with the correct one
#define DEFAULT_STAGE_LEVEL_COINS_QTY 30
#define DEFAULT_STAGE_LEVEL_COINS_COORD_Y 200
#define DEFAULT_STAGE_LEVEL_TIME 300
#define DEFAULT_STAGE_LEVEL_ENEMY_QTY 20
#define DEFAULT_STAGE_LEVEL_ENEMY_TYPE ENEMY_MUSHROOM
#define DEFAULT_STAGE_LEVEL_ENEMY_IMG "/home/file3.bmp" //TODO: Change file with the correct one
#define DEFAULT_STAGE_LEVEL_PLATFORM_TYPE PLATFORM_NORMAL
#define DEFAULT_STAGE_LEVEL_PLATFORM_COORD_X 100
#define DEFAULT_STAGE_LEVEL_PLATFORM_COORD_Y 200
#define DEFAULT_STAGE_LEVEL_PLATFORM_QTY 5

#define XML_LOG_LEVEL "configuracion.log.level"
#define XML_WINDOW_WIDTH "configuracion.ventana.ancho"
#define XML_WINDOW_HEIGHT "configuracion.ventana.alto"
#define XML_STAGE_LEVELS "configuracion.escenario.niveles"
#define XML_STAGE_LEVEL_NAME "nivel"
#define XML_STAGE_LEVEL_NUMBER "numero"
#define XML_STAGE_LEVEL_BACKGROUND "fondo"
#define XML_STAGE_LEVEL_COINS "monedas"
#define XML_STAGE_LEVEL_COIN_NAME "moneda"
#define XML_STAGE_LEVEL_TIME "tiempo"
#define XML_STAGE_LEVEL_COIN_COORDY "coordY"
#define XML_STAGE_LEVEL_COIN_QTY "cantidad"
#define XML_STAGE_LEVEL_ENEMIES "enemigos"
#define XML_STAGE_LEVEL_ENEMY_NAME "enemigo"
#define XML_STAGE_LEVEL_ENEMY_TYPE "tipo"
#define XML_STAGE_LEVEL_ENEMY_IMAGE "imagen"
#define XML_STAGE_LEVEL_ENEMY_QTY "cantidad"
#define XML_STAGE_LEVEL_PLATFORMS "plataformas"
#define XML_STAGE_LEVEL_PLATFORM_NAME "plataforma"
#define XML_STAGE_LEVEL_PLATFORM_TYPE "tipo"
#define XML_STAGE_LEVEL_PLATFORM_COORDX "coordX"
#define XML_STAGE_LEVEL_PLATFORM_COORDY "coordY"
#define XML_STAGE_LEVEL_PLATFORM_QTY "cantidad"

#include <vector>
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../logger/logger.h"

using namespace std;
using boost::property_tree::ptree;

struct Enemy {
    enemyType type;
    string image;
    int quantity;
};

struct Platform {
    platformType type;
    int coordX;
    int coordY;
    int quantity;
};

struct xmlCoin {
    int coordY;
    int quantity;
};

struct Level {
    int number;
    string background;
    int time;
    vector<xmlCoin> coins;
    vector<Enemy> enemies;
    vector<Platform> platforms;
};

struct Stage {
    vector<Level> levels;
};

struct Window {
    int width;
    int height;
};

struct Log {
    string level;
};

class Config {
public:
    Config();
    ~Config();

    Window getWindow();
    Stage getStage();
    Log getLog();

    void load(const std::string &filename);
    void setDefaults();
private:
    Window window;
    Stage stage;
    Log log;

    static void parseEnemies(Level *level, ptree pt);
    static void parsePlatforms(Level *level, ptree pt);
    static void parseCoins(Level *level, ptree pt);
};


#endif //CONFIG_H
