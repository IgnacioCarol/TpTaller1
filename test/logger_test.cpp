//
// Created by Daniel Bizari on 16/10/2020.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/logger.h"

using testing::Eq;

class LoggerTest : testing::Test {

};

TEST(LoggerTest, setLogLevel) {
    Logger * l = new Logger();

    l->setLogLevel(INFO);

    ASSERT_EQ(INFO, l->getLogLevel());
}

TEST(LoggerTest, setPathFile) {
    Logger * l = new Logger();

    l->setPathToFile("weon.txt");

    ASSERT_EQ("weon.txt", l->getPathToFile());
}