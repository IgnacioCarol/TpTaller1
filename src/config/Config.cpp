//
// Created by DANIELA CARRERO on 2020-10-15.
//

#include "Config.h"

Config::Config(void){
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

void Config::load(const std::string &filename) {
    ptree pt;

    try {
        read_xml(filename, pt);

        this->log.level = pt.get<string>(XML_LOG_LEVEL);

        this->window.width = pt.get<int>(XML_WINDOW_WIDTH);
        this->window.height = pt.get<int>(XML_WINDOW_HEIGHT);

        this->stage.levels.clear();
        for (const auto &l : pt.get_child(XML_STAGE_LEVELS)) {
            Level level;
            string level_name;
            ptree level_pt;
            tie(level_name, level_pt) = l;

            if (level_name != XML_STAGE_LEVEL_NAME) {
                continue;
            }

            level.number = level_pt.get<int>(XML_STAGE_LEVEL_NUMBER);
            level.background = level_pt.get<string>(XML_STAGE_LEVEL_BACKGROUND);

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
    for (const auto &e : pt.get_child(XML_STAGE_LEVEL_ENEMIES)) {
        xmlEnemy enemy;
        string enemy_name;
        ptree enemy_pt;
        tie(enemy_name, enemy_pt) = e;

        if (enemy_name != XML_STAGE_LEVEL_ENEMY_NAME) {
            continue;
        }

        string type = enemy_pt.get<string>(XML_STAGE_LEVEL_ENEMY_TYPE);
        if (type == XML_STAGE_LEVEL_ENEMY_MUSHROOM) {
            enemy.type = ENEMY_MUSHROOM;
        } else if (type == XML_STAGE_LEVEL_ENEMY_TURTLE) {
            enemy.type = ENEMY_TURTLE;
        } else {
            //TODO uso default o tiro una excepcion ?
            enemy.type = ENEMY_MUSHROOM;
        }

        enemy.image = enemy_pt.get<string>(XML_STAGE_LEVEL_ENEMY_IMAGE);
        enemy.quantity = enemy_pt.get<int>(XML_STAGE_LEVEL_ENEMY_QTY);
        level->enemies.push_back(enemy);
    }
}

void Config::parsePlatforms(Level *level, ptree pt) {
    for (const auto &e : pt.get_child(XML_STAGE_LEVEL_PLATFORMS)) {
        Platform platform;
        string platform_name;
        ptree platform_pt;
        tie(platform_name, platform_pt) = e;

        if (platform_name != XML_STAGE_LEVEL_PLATFORM_NAME) {
            continue;
        }

        string type = platform_pt.get<string>(XML_STAGE_LEVEL_PLATFORM_TYPE);
        if (type == XML_STAGE_LEVEL_PLATFORM_NORMAL) {
            platform.type = PLATFORM_NORMAL;
        } else if (type == XML_STAGE_LEVEL_PLATFORM_SURPRISE) {
            platform.type = PLATFORM_SURPRISE;
        } else {
            //TODO uso default o tiro una excepcion ?
            platform.type = PLATFORM_NORMAL;
        }

        platform.coordX = platform_pt.get<int>(XML_STAGE_LEVEL_PLATFORM_COORDX);
        platform.coordY = platform_pt.get<int>(XML_STAGE_LEVEL_PLATFORM_COORDY);
        platform.quantity = platform_pt.get<int>(XML_STAGE_LEVEL_PLATFORM_QTY);
        level->platforms.push_back(platform);
    }
}

void Config::parseCoins(Level *level, ptree pt) {
    for (const auto &e : pt.get_child(XML_STAGE_LEVEL_COINS)) {
        xmlCoin coin;
        string coin_name;
        ptree coin_pt;
        tie(coin_name, coin_pt) = e;

        if (coin_name != XML_STAGE_LEVEL_COIN_NAME) {
            continue;
        }

        coin.coordY = coin_pt.get<int>(XML_STAGE_LEVEL_COIN_COORDY);
        coin.quantity = coin_pt.get<int>(XML_STAGE_LEVEL_COIN_QTY);
        level->coins.push_back(coin);
    }
}


void Config::setDefaults() {
    xmlEnemy enemy;
    enemy.type = DEFAULT_STAGE_LEVEL_ENEMY_TYPE;
    enemy.image = DEFAULT_STAGE_LEVEL_ENEMY_IMG;
    enemy.quantity = DEFAULT_STAGE_LEVEL_ENEMY_QTY;

    Platform platform;
    platform.type = DEFAULT_STAGE_LEVEL_PLATFORM_TYPE;
    platform.coordX = DEFAULT_STAGE_LEVEL_PLATFORM_COORD_X;
    platform.coordY = DEFAULT_STAGE_LEVEL_PLATFORM_COORD_Y;
    platform.quantity = DEFAULT_STAGE_LEVEL_PLATFORM_QTY;

    xmlCoin coin;
    coin.coordY = DEFAULT_STAGE_LEVEL_COINS_COORD_Y;
    coin.quantity = DEFAULT_STAGE_LEVEL_COINS_QTY;

    Window window;
    window.width = DEFAULT_WINDOW_WIDTH;
    window.height = DEFAULT_WINDOW_HEIGHT;


    Log log;
    log.level = DEFAULT_LOG_LEVEL;

    Level level;
    level.number = DEFAULT_STAGE_LEVEL_NUMBER;
    level.background = DEFAULT_STAGE_LEVEL_BACKGROUND;
    level.enemies.push_back(enemy);
    level.platforms.push_back(platform);
    level.coins.push_back(coin);

    Stage stage;
    stage.levels.push_back(level);

    this->window = window;
    this->stage = stage;
    this->log = log;
}