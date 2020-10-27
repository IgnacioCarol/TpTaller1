//
// Created by Daniel Bizari on 26/10/2020.
//

#include "gtest/gtest.h"
#include "../src/config/Config.h"
#include "../src/Factory/Factory.h"
#include "../src/gameobjects/PlatformNormal.h"
#include "../src/gameobjects/PlatformSurprise.h"
#include "../src/gameobjects/EnemyTurtle.h"
#include "../src/gameobjects/EnemyMushroom.h"
#include "../src/gameobjects/Coin.h"
#include <vector>
#include <typeinfo>

using testing::Eq;

class FactoryTest : testing::Test {

};

TEST(FactoryTest, testAhre) {
    Config * config = new Config();
    PlatformNormal * pn = new PlatformNormal();
    PlatformSurprise * ps = new PlatformSurprise();
    EnemyMushroom * em = new EnemyMushroom();
    EnemyTurtle * et = new EnemyTurtle();
    Coin * c = new Coin();
    size_t count_pn = 0, count_ps = 0, count_em = 0, count_et = 0, count_c = 0;

    config->load("test/resources/config_test.xml");
    std::vector<GameObject *> result;
    result = Factory::getInstance()->createGameObjectsFromLevelConfig(config->getStage().levels.at(0));

    for(auto r : result) {
        if (typeid(*r) == typeid(*pn)) {
            count_pn++;
            continue;
        }
        if (typeid(*r) == typeid(*ps)) {
            count_ps++;
            continue;
        }
        if (typeid(*r) == typeid(*em)) {
            count_em++;
            continue;
        }
        if (typeid(*r) == typeid(*et)) {
            count_et++;
            continue;
        }
        if (typeid(*r) == typeid(*c)) {
            count_c++;
            continue;
        }
    }

    ASSERT_EQ(result.size(), count_c + count_et + count_em + count_ps + count_pn);
    ASSERT_EQ(16, count_c);
    ASSERT_EQ(10, count_em);
    ASSERT_EQ(5, count_et);
    ASSERT_EQ(2, count_pn);
    ASSERT_EQ(5, count_ps);
}