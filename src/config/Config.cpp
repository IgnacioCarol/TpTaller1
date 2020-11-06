//
// Created by DANIELA CARRERO on 2020-10-15.
//

#include "Config.h"

Config* Config::instance = nullptr;

Config::Config(){
    this->setDefaults();
}

Config::~Config() {

}

Window Config::getWindow() {
    return this->window;
}

Stage Config::getStage() {
    return this->stage;
}

Log Config::getLog() {
    return this->log;
}

Level Config::getLevel(int levelFilter) {
    for (const auto & level : this->stage.levels) {
        if (level.number == levelFilter) {
            return level;
        }
    }
    std::string errorMsg = "Couldn't find level " + std::to_string(levelFilter) + " in stage config";
    Logger::getInstance()->error(errorMsg);
    throw ConfigException(errorMsg);
}

void Config::load(const std::string &filename) {
    ptree pt;
    string errMsg;

    try {
        read_xml(filename, pt);

        this->log.level = pt.get<string>(XML_LOG_LEVEL);

        this->window.width = pt.get<int>(XML_WINDOW_WIDTH);
        if (this->window.width < 1) {
            errMsg = "Invalid windows width";
            Logger::getInstance()->error(errMsg);
            throw ConfigException(errMsg);
        }

        this->window.height = pt.get<int>(XML_WINDOW_HEIGHT);
        if (this->window.height < 1) {
            errMsg = "Invalid windows height";
            Logger::getInstance()->error(errMsg);
            throw ConfigException(errMsg);
        }

        this->stage.levels.clear();
        for (const auto &l : pt.get_child(XML_STAGE_LEVELS)) {
            Level level;
            string level_name;
            ptree level_pt;
            tie(level_name, level_pt) = l;

            if (level_name != XML_STAGE_LEVEL_NAME) {
                continue; //ToDo creo que aca habria que reportar error, preguntar a Dani C.
            }

            level.number = level_pt.get<int>(XML_STAGE_LEVEL_NUMBER);
            if (level.number < 1) {
                errMsg = "Invalid level number";
                Logger::getInstance()->error(errMsg);
                throw ConfigException(errMsg);
            }
            level.background = level_pt.get<string>(XML_STAGE_LEVEL_BACKGROUND);
            level.time = level_pt.get<int>(XML_STAGE_LEVEL_TIME);
            if (level.time < 1) {
                errMsg = "Invalid time configuration for level " + to_string(level.number);
                Logger::getInstance()->error(errMsg);
                throw ConfigException(errMsg);
            }

            level.enemies.clear();
            Config::parseEnemies(&level, level_pt);

            level.platforms.clear();
            Config::parsePlatforms(&level, level_pt);

            level.coins.clear();
            Config::parseCoins(&level, level_pt);

            this->stage.levels.push_back(level);
        }

    } catch (exception& ex) {
        string error_msg = "Error loading XML file config, error: ";
        Logger::getInstance()->error(error_msg + ex.what());
        this->setDefaults();
    }
}

void Config::parseEnemies(Level *level, ptree pt) {
    string errMsg;

    for (const auto &e : pt.get_child(XML_STAGE_LEVEL_ENEMIES)) {
        xmlEnemy enemy;
        string enemy_name;
        ptree enemy_pt;
        tie(enemy_name, enemy_pt) = e;

        if (enemy_name != XML_STAGE_LEVEL_ENEMY_NAME) {
            continue; //ToDo creo que aca habria que reportar error, preguntar a Dani C.
        }

        string type = enemy_pt.get<string>(XML_STAGE_LEVEL_ENEMY_TYPE);
        if (type == XML_STAGE_LEVEL_ENEMY_MUSHROOM) {
            enemy.type = ENEMY_MUSHROOM;
        } else if (type == XML_STAGE_LEVEL_ENEMY_TURTLE) {
            enemy.type = ENEMY_TURTLE;
        } else {
            Logger::getInstance()->info("Unknown enemy type " + type + " using mushroom as default");
            enemy.type = ENEMY_MUSHROOM; //If the type of enemy is not specified, it will show a mushroom.
        }

        enemy.image = enemy_pt.get<string>(XML_STAGE_LEVEL_IMAGE);
        enemy.quantity = enemy_pt.get<int>(XML_STAGE_LEVEL_ENEMY_QTY);
        if (enemy.quantity < 1) {
            errMsg = "Invalid enemy quantity " + to_string(enemy.quantity) + " for enemy type " + type;
            Logger::getInstance()->error(errMsg);
            throw ConfigException(errMsg);
        }

        level->enemies.push_back(enemy);
    }
}

void Config::parsePlatforms(Level *level, ptree pt) {
    string errMsg;

    for (const auto &e : pt.get_child(XML_STAGE_LEVEL_PLATFORMS)) {
        Platform platform;
        string platform_name;
        ptree platform_pt;
        tie(platform_name, platform_pt) = e;

        if (platform_name != XML_STAGE_LEVEL_PLATFORM_NAME) {
            continue; //ToDo creo que aca habria que reportar error, preguntar a Dani C.
        }

        string type = platform_pt.get<string>(XML_STAGE_LEVEL_PLATFORM_TYPE);
        if (type == XML_STAGE_LEVEL_PLATFORM_NORMAL) {
            platform.type = PLATFORM_NORMAL;
        } else if (type == XML_STAGE_LEVEL_PLATFORM_SURPRISE) {
            platform.type = PLATFORM_SURPRISE;
        } else {
            Logger::getInstance()->info("Unknown platform type " + type + " using normal platform as default");
            platform.type = PLATFORM_NORMAL; //If the type of the platforms is not specified, it will show a normal one.
        }

        platform.image = platform_pt.get<string>(XML_STAGE_LEVEL_IMAGE);
        platform.coordX = platform_pt.get<int>(XML_STAGE_LEVEL_PLATFORM_COORDX);
        platform.coordY = platform_pt.get<int>(XML_STAGE_LEVEL_PLATFORM_COORDY);
        platform.quantity = platform_pt.get<int>(XML_STAGE_LEVEL_PLATFORM_QTY);
        if (platform.quantity < 1) {
            errMsg = "Invalid platform quantity " + to_string(platform.quantity) + " for platform type " + type;
            Logger::getInstance()->error(errMsg);
            throw ConfigException(errMsg);
        }

        level->platforms.push_back(platform);
    }
}

void Config::parseCoins(Level *level, ptree pt) {
    string errMsg;

    for (const auto &e : pt.get_child(XML_STAGE_LEVEL_COINS)) {
        xmlCoin coin;
        string coin_name;
        ptree coin_pt;
        tie(coin_name, coin_pt) = e;

        if (coin_name != XML_STAGE_LEVEL_COIN_NAME) {
            continue; //ToDo creo que aca habria que reportar error, preguntar a Dani C.
        }

        coin.image = coin_pt.get<string>(XML_STAGE_LEVEL_IMAGE);
        coin.coordY = coin_pt.get<int>(XML_STAGE_LEVEL_COIN_COORDY);
        coin.quantity = coin_pt.get<int>(XML_STAGE_LEVEL_COIN_QTY);
        if (coin.quantity < 1) {
            errMsg = "Invalid coin quantity " + to_string(coin.quantity);
            Logger::getInstance()->error(errMsg);
            throw ConfigException(errMsg);
        }

        level->coins.push_back(coin);
    }
}


void Config::setDefaults() {
    Logger::getInstance()->info("Setting default config...");
    xmlEnemy enemy;
    enemy.type = DEFAULT_STAGE_LEVEL_ENEMY_TYPE;
    enemy.image = DEFAULT_STAGE_LEVEL_ENEMY_IMG;
    enemy.quantity = DEFAULT_STAGE_LEVEL_ENEMY_QTY;

    Platform platform;
    platform.image = DEFAULT_STAGE_LEVEL_PLATFORM_IMG;
    platform.type = DEFAULT_STAGE_LEVEL_PLATFORM_TYPE;
    platform.coordX = DEFAULT_STAGE_LEVEL_PLATFORM_COORD_X;
    platform.coordY = DEFAULT_STAGE_LEVEL_PLATFORM_COORD_Y;
    platform.quantity = DEFAULT_STAGE_LEVEL_PLATFORM_QTY;

    xmlCoin coin;
    coin.image = DEFAULT_STAGE_LEVEL_COIN_IMG;
    coin.coordY = DEFAULT_STAGE_LEVEL_COINS_COORD_Y;
    coin.quantity = DEFAULT_STAGE_LEVEL_COINS_QTY;

    Window window;
    window.width = DEFAULT_WINDOW_WIDTH;
    window.height = DEFAULT_WINDOW_HEIGHT;


    Log log;
    log.level = DEFAULT_LOG_LEVEL;

    Level level1;
    level1.number = DEFAULT_STAGE_FIRST_LEVEL_NUMBER;
    level1.background = DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND;
    level1.time = DEFAULT_STAGE_LEVEL_TIME;
    level1.enemies.push_back(enemy);
    level1.platforms.push_back(platform);
    level1.coins.push_back(coin);

    Level level2;
    level2.number = DEFAULT_STAGE_SECOND_LEVEL_NUMBER;
    level2.background = DEFAULT_STAGE_SECOND_LEVEL_BACKGROUND;
    level2.time = DEFAULT_STAGE_LEVEL_TIME;
    level2.enemies.push_back(enemy);
    level2.platforms.push_back(platform);
    level2.coins.push_back(coin);

    Level level3;
    level3.number = DEFAULT_STAGE_THIRD_LEVEL_NUMBER;
    level3.background = DEFAULT_STAGE_THIRD_LEVEL_BACKGROUND;
    level3.time = DEFAULT_STAGE_LEVEL_TIME;
    level3.enemies.push_back(enemy);
    level3.platforms.push_back(platform);
    level3.coins.push_back(coin);

    Stage stage;
    stage.levels.push_back(level1);
    stage.levels.push_back(level2);
    stage.levels.push_back(level3);

    this->window = window;
    this->stage = stage;
    this->log = log;
}