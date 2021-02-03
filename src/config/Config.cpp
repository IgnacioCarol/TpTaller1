#include "Config.h"

Config* Config::instance = nullptr;

Config::Config()= default;

Config::~Config() = default;

Window Config::getWindow() {
    return this->window;
}

Stage Config::getStage() {
    return this->stage;
}

Log Config::getLog() {
    return this->log;
}

Players Config::getPlayers() {
    return this->players;
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
    try {
        read_xml(filename, pt);

        validateTags(XML_CONFIG_TAG, validGeneralTags, pt);
        validateTags(XML_CONFIG_TAG, validConfigTags, pt.get_child(XML_CONFIG_TAG));

        parseLog(pt);
        Logger::getInstance()->setLogLevel(this->log.level);

        parseWindow(pt);
        parseStage(pt);
        parsePlayers(pt);

        Logger::getInstance()->info("XML: " + filename + " loaded successfully");

    } catch (exception& ex) {
        string error_msg = "Error loading XML file config, error: ";
        Logger::getInstance()->error(error_msg + ex.what());
        this->setDefaults();
    }

    Logger::getInstance()->setLogLevel(this->log.level);
}

void Config::parseLog(ptree pt) {
    validateTags(XML_LOG_TAG, validLogTags, pt.get_child(XML_LOG_TAG));
    string logLevel = pt.get<string>(XML_LOG_LEVEL);
    if (!(std::find(validLogLevels.begin(), validLogLevels.end(), logLevel) != validLogLevels.end())) {
        throw ConfigException("Unknown log level type " + logLevel);
    }
    this->log.level = logLevel;
}

void Config::parseWindow(ptree pt) {
    validateTags(XML_WINDOW_TAG, validWindowTags, pt.get_child(XML_WINDOW_TAG));

    this->window.width = pt.get<int>(XML_WINDOW_WIDTH);
    if (this->window.width < 1) {
        Logger::getInstance()->error("Invalid windows width "+ to_string(window.width) + ". Expected greater than 0, setting default to 0");
        this->window.width = 0;
    }

    this->window.height = pt.get<int>(XML_WINDOW_HEIGHT);
    if (this->window.height < 1) {
        Logger::getInstance()->error("Invalid windows height " + to_string(window.height) + ". Expected greater than 0, setting default to 0");
        this->window.height = 0;
    }
}

void Config::parseStage(ptree pt) {
    validateTags(XML_STAGE_TAG, validStageTags, pt.get_child(XML_STAGE_TAG));
    this->stage.levels.clear();
    for (const auto &l : pt.get_child(XML_STAGE_LEVELS)) {
        Level level;
        string level_name;
        ptree level_pt;
        tie(level_name, level_pt) = l;

        if (level_name != XML_STAGE_LEVEL_NAME) {
            throw ConfigException("Invalid level tag, found: " + level_name);
        }

        validateTags(XML_STAGE_LEVEL_NAME, validLevelTags, level_pt);

        level.number = level_pt.get<int>(XML_STAGE_LEVEL_NUMBER);
        if (level.number < 1) {
            Logger::getInstance()->error("Invalid level number " + to_string(level.number) + ". Expected greater than 0, setting default to 1");
            level.number = 1;
        }

        level.background = level_pt.get<string>(XML_STAGE_LEVEL_BACKGROUND);
        level.time = level_pt.get<int>(XML_STAGE_LEVEL_TIME);
        if (level.time < 1) {
            Logger::getInstance()->error("Invalid time configuration for level " + to_string(level.time) + ". Expected greater than 0, setting default to 0");
            level.time = 300;
        }

        level.enemies.clear();
        Config::parseEnemies(&level, level_pt);

        level.platforms.clear();
        Config::parsePlatforms(&level, level_pt);

        level.coins.clear();
        Config::parseCoins(&level, level_pt);

        level.coins.clear();
        Config::parseHoles(&level, level_pt);

        this->stage.levels.push_back(level);
    }
}

void Config::parseEnemies(Level *level, ptree pt) {
    for (const auto &e : pt.get_child(XML_STAGE_LEVEL_ENEMIES)) {
        xmlEnemy enemy;
        string enemy_name;
        ptree enemy_pt;
        tie(enemy_name, enemy_pt) = e;

        if (enemy_name != XML_STAGE_LEVEL_ENEMY_NAME) {
            throw ConfigException("Invalid enemy tag, found: " + enemy_name);
        }

        validateTags(XML_STAGE_LEVEL_ENEMY_NAME, validEnemyTags, enemy_pt);

        string type = enemy_pt.get<string>(XML_STAGE_LEVEL_ENEMY_TYPE);
        if (type == XML_STAGE_LEVEL_ENEMY_MUSHROOM) {
            enemy.type = ENEMY_MUSHROOM;
        } else if (type == XML_STAGE_LEVEL_ENEMY_TURTLE) {
            enemy.type = ENEMY_TURTLE;
        } else {
            throw ConfigException("Unknown enemy type " + type);
        }

        enemy.image = enemy_pt.get<string>(XML_STAGE_LEVEL_IMAGE);
        enemy.quantity = enemy_pt.get<int>(XML_STAGE_LEVEL_ENEMY_QTY);
        if (enemy.quantity < 1) {
            Logger::getInstance()->error("Invalid enemy quantity " + to_string(enemy.quantity) + " for enemy type " + type
                                         + ". Expected greater than 0, setting default to 0");
            enemy.quantity = 0;
        }

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
            throw ConfigException("Invalid platform tag, found: " + platform_name);
        }

        validateTags(XML_STAGE_LEVEL_PLATFORM_NAME, validPlatformTags, platform_pt);

        string type = platform_pt.get<string>(XML_STAGE_LEVEL_PLATFORM_TYPE);
        if (type == XML_STAGE_LEVEL_PLATFORM_NORMAL) {
            platform.type = PLATFORM_NORMAL;
        } else if (type == XML_STAGE_LEVEL_PLATFORM_SURPRISE) {
            platform.type = PLATFORM_SURPRISE;
        } else {
            throw ConfigException("Unknown platform type " + type);
        }

        platform.image = platform_pt.get<string>(XML_STAGE_LEVEL_IMAGE);
        platform.coordX = platform_pt.get<int>(XML_STAGE_LEVEL_PLATFORM_COORDX);
        if (platform.coordX < 0) {
            Logger::getInstance()->error("Invalid platform coordX " + to_string(platform.coordX) + " for platform type " + type
                                         + ". Expected greater than 0, setting default to 0");
            platform.coordX = 0;
        }
        platform.coordY = platform_pt.get<int>(XML_STAGE_LEVEL_PLATFORM_COORDY);
        if (platform.coordY < 1) {
            Logger::getInstance()->error("Invalid platform coordY " + to_string(platform.coordY) + " for platform type " + type
                                         + ". Expected greater than 0, setting default to 0");
            platform.coordY = 0;
        }
        platform.quantity = platform_pt.get<int>(XML_STAGE_LEVEL_PLATFORM_QTY);
        if (platform.quantity < 1) {
            Logger::getInstance()->error("Invalid platform quantity " + to_string(platform.quantity) + " for platform type " + type
                                         + ". Expected greater than 0, setting default to 0");
            platform.quantity = 0;
        }

        level->platforms.push_back(platform);
    }
}

void Config::parseHoles(Level *level, ptree pt) {
    for (const auto &e : pt.get_child(XML_STAGE_LEVEL_HOLES)) {
        xmlHole hole;
        string hole_name;
        ptree hole_pt;
        tie(hole_name, hole_pt) = e;

        if (hole_name != XML_STAGE_LEVEL_HOLE_NAME) {
            throw ConfigException("Invalid hole tag, found: " + hole_name);
        }

        validateTags(XML_STAGE_LEVEL_HOLE_NAME, validHoleTags, hole_pt);

        hole.coordX = hole_pt.get<int>(XML_STAGE_LEVEL_HOLE_COORDX);
        if (hole.coordX < 0) {
            Logger::getInstance()->error("Invalid hole coordX " + to_string(hole.coordX)
                                         + ". Expected greater than 0, setting default to 0");
            hole.coordX = 0;
        }
        hole.coordY = hole_pt.get<int>(XML_STAGE_LEVEL_HOLE_COORDY);
        if (hole.coordY < 1) {
            Logger::getInstance()->error("Invalid hole coordY " + to_string(hole.coordY)
                                         + ". Expected greater than 0, setting default to 0");
            hole.coordY = 0;
        }
        hole.width = hole_pt.get<int>(XML_STAGE_LEVEL_HOLE_WIDTH);
        if (hole.width < 1) {
            Logger::getInstance()->error("Invalid hole width " + to_string(hole.width)
                                         + ". Expected greater than 0, setting default to 0");
            hole.width = 0;
        }
        hole.height = hole_pt.get<int>(XML_STAGE_LEVEL_HOLE_HEIGHT);
        if (hole.height < 1) {
            Logger::getInstance()->error("Invalid hole height " + to_string(hole.height)
                                         + ". Expected greater than 0, setting default to 0");
            hole.height = 0;
        }

        level->holes.push_back(hole);
    }
}


void Config::parseCoins(Level *level, ptree pt) {
    for (const auto &e : pt.get_child(XML_STAGE_LEVEL_COINS)) {
        xmlCoin coin;
        string coin_name;
        ptree coin_pt;
        tie(coin_name, coin_pt) = e;

        if (coin_name != XML_STAGE_LEVEL_COIN_NAME) {
            throw ConfigException("Invalid coin tag, found: " + coin_name);
        }

        validateTags(XML_STAGE_LEVEL_COIN_NAME, validCoinTags, coin_pt);

        coin.image = coin_pt.get<string>(XML_STAGE_LEVEL_IMAGE);
        coin.coordY = coin_pt.get<int>(XML_STAGE_LEVEL_COIN_COORDY);
        if (coin.coordY < 0) {
            Logger::getInstance()->error("Invalid coin coordY " + to_string(coin.coordY) + ". Expected greater than 0, setting default to 0");
            coin.coordY = 0;
        }
        coin.quantity = coin_pt.get<int>(XML_STAGE_LEVEL_COIN_QTY);
        if (coin.quantity < 1) {
            Logger::getInstance()->error("Invalid coin quantity " + to_string(coin.quantity) + ". Expected greater than 0, setting default to 0");
            coin.quantity = 0;
        }

        level->coins.push_back(coin);
    }
}

void Config::parsePlayers(ptree pt) {
    validateTags(XML_CREDENTIAL_TAG, validCredentialTags, pt.get_child(XML_CREDENTIAL_TAG));
    this->players.amount = pt.get<int>(XML_CREDENTIAL_CONNECTIONS);
    if (players.amount< 1) {
        Logger::getInstance()->error("Invalid amount of players " + to_string(players.amount) + ". Expected greater than 0, setting default to 1");
        players.amount = 1;
    }
    this->players.users.clear();
    for (const auto &u : pt.get_child(XML_CREDENTIAL_USERS_SECTION)) {
        User user;
        string credential_user;
        ptree user_pt;
        tie(credential_user, user_pt) = u;

        if (credential_user != XML_CREDENTIAL_USER) {
            throw ConfigException("Invalid user tag, found: " + credential_user);
        }

        validateTags(XML_CREDENTIAL_USER, validUserTags, user_pt);

        user.username = user_pt.get<string>(XML_CREDENTIAL_USERS_NAME);
        user.password = user_pt.get<string>(XML_CREDENTIAL_USERS_PASSWORD);

        this->players.users.push_back(user);
    }
}

void Config::setDefaults() {
    Logger::getInstance()->info("Setting default config...");
    defaultConfig = true;

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

    User user;
    user.username = DEFAULT_USER_USERNAME;
    user.password = DEFAULT_USER_PASSWORD;

    Players players;
    players.amount = 1;
    players.users.push_back(user);

    this->window = window;
    this->stage = stage;
    this->log = log;
    this->players = players;

    //TODO default de usuarios

}

void Config::validateTags(string xmlLvl, vector<string> validTags, ptree pt) {
    for (auto& it: pt) {
        string level_name;
        ptree level_pt;
        tie(level_name, level_pt) = it;

        if (!(std::find(validTags.begin(), validTags.end(), level_name) != validTags.end())) {
            throw ConfigException("Unexpected tag in " + xmlLvl + ": " + level_name);
        }
    }
}

bool Config::isDefault() {
    return this->defaultConfig;
}
