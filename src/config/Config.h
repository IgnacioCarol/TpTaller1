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

#define XML_LOG_LEVEL "configuracion.log.level"
#define XML_WINDOW_WIDTH "configuracion.ventana.ancho"
#define XML_WINDOW_HEIGHT "configuracion.ventana.alto"
#define XML_STAGE_LEVELS "configuracion.escenario.niveles"
#define XML_STAGE_LEVEL_NAME "nivel"
#define XML_STAGE_LEVEL_NUMBER "numero"
#define XML_STAGE_LEVEL_BACKGROUND "fondo"
#define XML_STAGE_LEVEL_COINS "monedas"
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

#include <list>
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;
using boost::property_tree::ptree;

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
    int number = DEFAULT_STAGE_LEVEL_NUMBER;
    string background = DEFAULT_STAGE_LEVEL_BACKGROUND;
    int coins = DEFAULT_STAGE_LEVEL_COINS;
    list<Enemy> enemies;
    list<Platform> platforms;
};

struct Stage {
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
    Log getLog();

    void load(const std::string &filename);
    void setDefaults();
private:
    Window window;
    Stage stage;
    Log log;

    static void parseEnemies(Level *level, ptree pt);
    static void parsePlatforms(Level *level, ptree pt);
};


#endif //CONFIG_H
