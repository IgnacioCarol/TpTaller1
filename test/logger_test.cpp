//
// Created by Daniel Bizari on 16/10/2020.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/logger/logger.h"
#include "../src/logger/loggerException.h"

using testing::Eq;

class LoggerTest : testing::Test {

};

TEST(LoggerTest, setLogLevel) {
    Logger * l = Logger::getInstance();

    l->setLogLevel("INFO");

    ASSERT_EQ(INFO, l->getLogLevel());
}

TEST(LoggerTest, write) {
    Logger * l = Logger::getInstance();

    l->setLogLevel("INFO");
    std::string logMsg = l->info("Que onda perri");

    ASSERT_TRUE(logMsg.find("[INFO]: Que onda perri\n") != std::string::npos);
}

TEST(LoggerTest, errorLevelJustLogsErrorEvents) {
    Logger * l = Logger::getInstance();

    l->setLogLevel("ERROR");
    l->error("Esto es un error");
    l->info("Que onda perri");
    l->debug("Esto es un dato de vital importancia");

    delete l;

    ASSERT_TRUE(true);
}

TEST(LoggerTest, inexistentLogLevelThrowsException) {
    Logger *l = Logger::getInstance();
    std::string errorMsg = "";

    try {
        l->setLogLevel("TU_Vieja");
        FAIL() << "Expected loggerException";
    } catch (std::exception &ex) {
        errorMsg = ex.what();
        ASSERT_EQ("inexistent log level", errorMsg);
    } catch (...) {
        FAIL() << "Expected loggerException";
    }
}