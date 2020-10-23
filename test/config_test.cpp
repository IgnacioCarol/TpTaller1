//
// Created by DANIELA CARRERO on 2020-10-18.
//

#include "gtest/gtest.h"
#include "../src/config/Config.h"

using testing::Eq;

static void assertDefaults(Config config);

class ConfigTest : testing::Test {

};

TEST(ConfigTest, setDefaults) {
    Config config;

    config.setDefaults();
    assertDefaults(config);
}

TEST(ConfigTest, setDefaultsIfFileDoesNotExists) {
    Config config;

    config.load("unexistingFile");
    assertDefaults(config);
}

TEST(ConfigTest, loadXMLConfig) {
    Config config;

    config.load("test/resources/config_test.xml");

    ASSERT_EQ("DEBUG", config.getLog().level);

    ASSERT_EQ(400, config.getWindow().height);
    ASSERT_EQ(300, config.getWindow().width);

    ASSERT_EQ(1, config.getStage().levels.size());
    ASSERT_EQ(10, config.getStage().levels.back().coins);
    ASSERT_EQ("/home/test_file1.bmp", config.getStage().levels.back().background);
    ASSERT_EQ(2, config.getStage().levels.back().number);

    ASSERT_EQ(2, config.getStage().levels.back().enemies.size());
    Enemy enemy1 = config.getStage().levels.back().enemies.front();
    Enemy enemy2 = config.getStage().levels.back().enemies.back();
    ASSERT_EQ(10, enemy1.quantity);
    ASSERT_EQ("/home/test_file2.bmp", enemy1.image);
    ASSERT_EQ(1, enemy1.type);
    ASSERT_EQ(5, enemy2.quantity);
    ASSERT_EQ("/home/test_file3.bmp", enemy2.image);
    ASSERT_EQ(2, enemy2.type);

    ASSERT_EQ(2, config.getStage().levels.back().platforms.size());
    Platform platform1 = config.getStage().levels.back().platforms.front();
    Platform platform2 = config.getStage().levels.back().platforms.back();
    ASSERT_EQ("ladrillo_test", platform1.type);
    ASSERT_EQ(500, platform1.coordX);
    ASSERT_EQ(300, platform1.coordY);
    ASSERT_EQ(2, platform1.quantity);
    ASSERT_EQ("sorpresa_test", platform2.type);
    ASSERT_EQ(400, platform2.coordX);
    ASSERT_EQ(500, platform2.coordY);
    ASSERT_EQ(5, platform2.quantity);
}

static void assertDefaults(Config config) {
    ASSERT_EQ(DEFAULT_LOG_LEVEL, config.getLog().level);

    ASSERT_EQ(DEFAULT_WINDOW_HEIGHT, config.getWindow().height);
    ASSERT_EQ(DEFAULT_WINDOW_WIDTH, config.getWindow().width);

    ASSERT_EQ(1, config.getStage().levels.size());
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_COINS, config.getStage().levels.back().coins);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_BACKGROUND, config.getStage().levels.back().background);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_NUMBER, config.getStage().levels.back().number);

    ASSERT_EQ(1, config.getStage().levels.back().enemies.size());
    Enemy enemy = config.getStage().levels.back().enemies.back();
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_ENEMY_QTY, enemy.quantity);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_ENEMY_IMG, enemy.image);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_ENEMY_TYPE, enemy.type);

    ASSERT_EQ(1, config.getStage().levels.back().platforms.size());
    Platform platform = config.getStage().levels.back().platforms.back();
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_PLATFORM_TYPE, platform.type);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_PLATFORM_COORD_X, platform.coordX);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_PLATFORM_COORD_Y, platform.coordY);
    ASSERT_EQ(DEFAULT_STAGE_LEVEL_PLATFORM_QTY, platform.quantity);
}