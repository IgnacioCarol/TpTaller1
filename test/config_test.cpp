//
// Created by DANIELA CARRERO on 2020-10-18.
//

#include "gtest/gtest.h"
#include "../src/config/Config.h"

using testing::Eq;

static void assertDefaults(Config* config);

class ConfigTest : testing::Test {

};

TEST(ConfigTest, setDefaults) {
    Config *config = Config::getInstance();

    config->setDefaults();
    assertDefaults(config);
}

TEST(ConfigTest, setDefaultsIfFileDoesNotExists) {
    Config *config = Config::getInstance();

    config->load("unexistingFile");
    assertDefaults(config);
}

TEST(ConfigTest, invalidWidthSize) {
    Config *config = Config::getInstance();

    config->load("test/resources/config_test_broken_width.xml");
    assertDefaults(config);
}

TEST(ConfigTest, invalidHeightSize) {
    Config *config = Config::getInstance();

    config->load("test/resources/config_test_broken_height.xml");
    assertDefaults(config);
}

TEST(ConfigTest, invalidLevelNumber) {
    Config *config = Config::getInstance();

    config->load("test/resources/config_test_broken_level_number.xml");
    assertDefaults(config);
}

TEST(ConfigTest, invalidTime) {
    Config *config = Config::getInstance();

    config->load("test/resources/config_test_broken_time.xml");
    assertDefaults(config);
}

TEST(ConfigTest, invalidEnemyQuantity) {
    Config *config = Config::getInstance();

    config->load("test/resources/config_test_broken_enemy_quantity.xml");
    assertDefaults(config);
}

TEST(ConfigTest, invalidCoinQuantity) {
    Config *config = Config::getInstance();

    config->load("test/resources/config_test_broken_coin_quantity.xml");
    assertDefaults(config);
}

TEST(ConfigTest, invalidPlatformQuantity) {
    Config *config = Config::getInstance();

    config->load("test/resources/config_test_broken_platform_quantity.xml");
    assertDefaults(config);
}

TEST(ConfigTest, loadXMLConfig) {
    Config *config = Config::getInstance();

    config->load("test/resources/config_test.xml");

    ASSERT_EQ("DEBUG", config->getLog().level);

    ASSERT_EQ(400, config->getWindow().height);
    ASSERT_EQ(300, config->getWindow().width);

    ASSERT_EQ(1, config->getStage().levels.size());
    ASSERT_EQ("/home/test_file1.bmp", config->getStage().levels.back().background);
    ASSERT_EQ(2, config->getStage().levels.back().number);

    ASSERT_EQ(2, config->getStage().levels.back().enemies.size());
    xmlEnemy enemy1 = config->getStage().levels.back().enemies.front();
    xmlEnemy enemy2 = config->getStage().levels.back().enemies.back();
    ASSERT_EQ(10, enemy1.quantity);
    ASSERT_EQ("/home/test_file2.bmp", enemy1.image);
    ASSERT_EQ(ENEMY_MUSHROOM, enemy1.type);
    ASSERT_EQ(5, enemy2.quantity);
    ASSERT_EQ("/home/test_file3.bmp", enemy2.image);
    ASSERT_EQ(ENEMY_TURTLE, enemy2.type);

    ASSERT_EQ(2, config->getStage().levels.back().coins.size());
    xmlCoin coin1 = config->getStage().levels.back().coins.front();
    xmlCoin coin2 = config->getStage().levels.back().coins.back();
    ASSERT_EQ(13, coin1.quantity);
    ASSERT_EQ(10, coin1.coordY);
    ASSERT_EQ(3, coin2.quantity);
    ASSERT_EQ(20, coin2.coordY);

    ASSERT_EQ(2, config->getStage().levels.back().platforms.size());
    Platform platform1 = config->getStage().levels.back().platforms.front();
    Platform platform2 = config->getStage().levels.back().platforms.back();
    ASSERT_EQ(PLATFORM_NORMAL, platform1.type);
    ASSERT_EQ(500, platform1.coordX);
    ASSERT_EQ(300, platform1.coordY);
    ASSERT_EQ(2, platform1.quantity);
    ASSERT_EQ(PLATFORM_SURPRISE, platform2.type);
    ASSERT_EQ(400, platform2.coordX);
    ASSERT_EQ(500, platform2.coordY);
    ASSERT_EQ(5, platform2.quantity);
}

static void assertDefaults(Config* config) {
    ASSERT_EQ(DEFAULT_LOG_LEVEL, config->getLog().level);

    ASSERT_EQ(DEFAULT_WINDOW_HEIGHT, config->getWindow().height);
    ASSERT_EQ(DEFAULT_WINDOW_WIDTH, config->getWindow().width);

    ASSERT_EQ(3, config->getStage().levels.size());
    ASSERT_EQ(DEFAULT_STAGE_FIRST_LEVEL_BACKGROUND, config->getStage().levels.at(0).background);
    ASSERT_EQ(DEFAULT_STAGE_FIRST_LEVEL_NUMBER, config->getStage().levels.at(0).number);

    ASSERT_EQ(1, config->getStage().levels.back().enemies.size());
    xmlEnemy enemy = config->getStage().levels.back().enemies.back();
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_ENEMY_QTY, enemy.quantity);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_ENEMY_IMG, enemy.image);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_ENEMY_TYPE, enemy.type);

    ASSERT_EQ(1, config->getStage().levels.back().platforms.size());
    Platform platform = config->getStage().levels.back().platforms.back();
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_PLATFORM_TYPE, platform.type);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_PLATFORM_COORD_X, platform.coordX);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_PLATFORM_COORD_Y, platform.coordY);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_PLATFORM_QTY, platform.quantity);

    ASSERT_EQ(1, config->getStage().levels.back().enemies.size());
    xmlCoin coin = config->getStage().levels.back().coins.back();
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_COINS_QTY, coin.quantity);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_COINS_COORD_Y, coin.coordY);
}