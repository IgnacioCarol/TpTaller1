//
// Created by Daniel Bizari on 26/10/2020.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/GameMap.h"
#include "../src/gameobjects/GameObject.h"
#include "../src/gameobjects/Coin.h"

using testing::Eq;

class GameMapTest : testing::Test {

};

TEST(GameMapTest, insertNewGameObject) {
    GameMap * map = GameMap::getInstance();
    GameObject * actor = new Coin();

    bool response = map->insertTo(1, 2, actor);

    ASSERT_EQ(true, response);
}

TEST(GameMapTest, insertRepeatedGameObjectReturnFalse) {
    GameMap * map = GameMap::getInstance();
    GameObject * actor1 = new Coin();
    GameObject * actor2 = new Coin();

    bool response1 = map->insertTo(1, 3, actor1);//Todo quiza estaria bueno que el init del gameObject llame al gameMap
    bool response2 = map->insertTo(1, 3, actor2);

    ASSERT_EQ(true, response1);
    ASSERT_EQ(false, response2);
}

TEST(GameMapTest, getRandomX) {
    GameMap * map = GameMap::getInstance();
    GameObject * actor = new Coin();

    bool response1 = map->insertTo(1, 3, actor);
    size_t x = map->getRandomX(3);

    ASSERT_EQ(true, response1);
    ASSERT_GT(x, 0);
    std::cout << "x: " << x << " y: " << 3 << std::endl;
}