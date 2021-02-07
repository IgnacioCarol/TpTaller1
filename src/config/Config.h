#ifndef CONFIG_H
#define CONFIG_H

#define XML_STAGE_LEVEL_ENEMY_TURTLE   "enemigo_tortuga"
#define XML_STAGE_LEVEL_ENEMY_MUSHROOM "enemigo_hongo"
typedef enum{ENEMY_MUSHROOM, ENEMY_TURTLE} enemyType;


#define XML_STAGE_LEVEL_PLATFORM_NORMAL   "plataforma_normal"
#define XML_STAGE_LEVEL_PLATFORM_SURPRISE "plataforma_sorpresa"
typedef enum{PLATFORM_NORMAL, PLATFORM_SURPRISE} platformType;

#define DEFAULT_LOG_LEVEL "DEBUG"
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

#define DEFAULT_STAGE_FIRST_LEVEL_NUMBER 1
#define DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND "Sprites/Backgrounds/level1.png"

#define DEFAULT_STAGE_SECOND_LEVEL_NUMBER 2
#define DEFAULT_STAGE_SECOND_LEVEL_BACKGROUND "Sprites/Backgrounds/level2.png"

#define DEFAULT_STAGE_THIRD_LEVEL_NUMBER 3
#define DEFAULT_STAGE_THIRD_LEVEL_BACKGROUND "Sprites/Backgrounds/level3.png"

#define DEFAULT_STAGE_LEVEL_COINS_QTY 30
#define DEFAULT_STAGE_LEVEL_COINS_COORD_Y 150
#define DEFAULT_STAGE_LEVEL_TIME 300
#define DEFAULT_STAGE_LEVEL_ENEMY_QTY 20
#define DEFAULT_STAGE_LEVEL_ENEMY_TYPE ENEMY_MUSHROOM
#define DEFAULT_STAGE_LEVEL_ENEMY_IMG "Sprites/Goomba.png"
#define DEFAULT_STAGE_LEVEL_PLATFORM_TYPE PLATFORM_NORMAL
#define DEFAULT_STAGE_LEVEL_PLATFORM_COORD_X 100
#define DEFAULT_STAGE_LEVEL_PLATFORM_COORD_Y 200
#define DEFAULT_STAGE_LEVEL_PLATFORM_QTY 5

#define DEFAULT_STAGE_LEVEL_PLATFORM_IMG "Sprites/normalBlock.png"
#define DEFAULT_STAGE_LEVEL_COIN_IMG "Sprites/coinsSprites.png"

#define DEFAULT_USER_USERNAME "mario"
#define DEFAULT_USER_PASSWORD "mario"

#define XML_CONFIG_TAG "configuracion"
#define XML_LOG_TAG "configuracion.log"
#define XML_WINDOW_TAG "configuracion.ventana"
#define XML_STAGE_TAG "configuracion.escenario"
#define XML_LOG_LEVEL "configuracion.log.level"
#define XML_WINDOW_WIDTH "configuracion.ventana.ancho"
#define XML_WINDOW_HEIGHT "configuracion.ventana.alto"
#define XML_STAGE_LEVELS "configuracion.escenario.niveles"
#define XML_STAGE_LEVEL_NAME "nivel"
#define XML_STAGE_LEVEL_NUMBER "numero"
#define XML_STAGE_LEVEL_BACKGROUND "fondo"
#define XML_STAGE_LEVEL_IMAGE "imagen"
#define XML_STAGE_LEVEL_COINS "monedas"
#define XML_STAGE_LEVEL_COIN_NAME "moneda"
#define XML_STAGE_LEVEL_TIME "tiempo"
#define XML_STAGE_LEVEL_COIN_COORDY "coordY"
#define XML_STAGE_LEVEL_COIN_QTY "cantidad"
#define XML_STAGE_LEVEL_ENEMIES "enemigos"
#define XML_STAGE_LEVEL_ENEMY_NAME "enemigo"
#define XML_STAGE_LEVEL_ENEMY_TYPE "tipo"
#define XML_STAGE_LEVEL_ENEMY_QTY "cantidad"
// Holes
#define XML_STAGE_LEVEL_HOLES "pozos"
#define XML_STAGE_LEVEL_HOLE_NAME "pozo"
#define XML_STAGE_LEVEL_HOLE_COORDX "coordX"
#define XML_STAGE_LEVEL_HOLE_COORDY "coordY"
#define XML_STAGE_LEVEL_HOLE_WIDTH "ancho"
#define XML_STAGE_LEVEL_HOLE_HEIGHT "alto"
// Platforms
#define XML_STAGE_LEVEL_PLATFORMS "plataformas"
#define XML_STAGE_LEVEL_PLATFORM_NAME "plataforma"
#define XML_STAGE_LEVEL_PLATFORM_TYPE "tipo"
#define XML_STAGE_LEVEL_PLATFORM_COORDX "coordX"
#define XML_STAGE_LEVEL_PLATFORM_COORDY "coordY"
#define XML_STAGE_LEVEL_PLATFORM_QTY "cantidad"
// Pipes
#define XML_STAGE_LEVEL_PIPES "tubos"
#define XML_STAGE_LEVEL_PIPE_NAME "tubo"
#define XML_STAGE_LEVEL_PIPE_COORDX "coordX"
#define XML_STAGE_LEVEL_PIPE_COORDY "coordY"
#define XML_STAGE_LEVEL_PLATFORM_QTY "cantidad"

#define XML_CREDENTIAL_TAG "configuracion.credenciales"
#define XML_CREDENTIAL_CONNECTIONS "configuracion.credenciales.cantidadJugadores"
#define XML_CREDENTIAL_USERS_SECTION "configuracion.credenciales.usuarios"
#define XML_CREDENTIAL_USER "usuario"
#define XML_CREDENTIAL_USERS_NAME "nombre"
#define XML_CREDENTIAL_USERS_PASSWORD "contrasenia"

#include <vector>
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../logger/logger.h"
#include "ConfigException.h"

using namespace std;
using boost::property_tree::ptree;

struct xmlEnemy {
    enemyType type;
    string image;
    int quantity;
};

struct Platform {
    platformType type;
    string image;
    int coordX;
    int coordY;
    int quantity;
};

struct xmlHole {
    int coordX;
    int coordY;
    int width;
    int height;
};

struct xmlPipe {
    int coordX;
    int coordY;
    string image;
};

struct xmlCoin { //ToDo el xml no deberia tener un path por moneda, reconfigurar para que reciba el path de las imagenes una sola vez
    string image;
    int coordY;
    int quantity;
};

struct Level {
    int number;
    string background;
    int time;
    vector<xmlCoin> coins;
    vector<xmlEnemy> enemies;
    vector<Platform> platforms;
    vector<xmlHole>  holes;
    vector<xmlPipe>  pipes;
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

struct User {
    string username;
    string password;
};

struct Players {
    int amount;
    vector<User> users;
};

class Config {
public:
    static Config* getInstance() {
        if (instance == nullptr) {
            instance = new Config();
        }
        return instance;
    }

    ~Config();

    Window getWindow();
    Stage getStage();
    Log getLog();
    Level getLevel(int level);
    Players getPlayers();
    bool isDefault();

    void load(const std::string &filename);
    void setDefaults();
private:
    static Config* instance;
    Window window{};
    Stage stage;
    Players players;
    Log log;
    bool defaultConfig = false;

    Config();
    void parseLog(ptree pt);
    void parseWindow(ptree pt);
    void parseStage(ptree pt);
    void parseEnemies(Level *level, ptree pt);
    void parsePlatforms(Level *level, ptree pt);
    void parseHoles(Level *level, ptree pt);
    void parsePipes(Level *level, ptree pt);
    void parseCoins(Level *level, ptree pt);
    void parsePlayers(ptree pt);
    void validateTags(string xmlLvl, vector<string> validTags, ptree pt);

    const vector<string> validGeneralTags = {"configuracion"};
    const vector<string> validConfigTags = {"log", "ventana", "escenario", "credenciales"};
    const vector<string> validLogTags = {"level"};
    const vector<string> validWindowTags = {"ancho", "alto"};
    const vector<string> validStageTags = {"niveles"};
    const vector<string> validCredentialTags = {"usuarios", "cantidadJugadores"};
    const vector<string> validUserTags = {"nombre", "contrasenia"};
    const vector<string> validLevelTags = {"numero", "fondo", "monedas", "tiempo", "enemigos", "plataformas", "pozos", "tubos"};
    const vector<string> validCoinTags = {"imagen", "coordY", "cantidad"};
    const vector<string> validEnemyTags = {"tipo", "imagen", "cantidad"};
    const vector<string> validPlatformTags = {"tipo", "imagen", "coordX", "coordY", "cantidad"};
    const vector<string> validHoleTags = {"coordX", "coordY", "ancho", "alto"};
    const vector<string> validPipeTags = {"coordX", "coordY", "imagen"};
    const vector<string> validLogLevels = {"ERROR", "DEBUG", "INFO"};
};


#endif //CONFIG_H
