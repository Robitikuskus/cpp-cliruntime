#include <gtest/gtest.h>
#include "command_handler.hpp"
#include "logger.hpp"

TEST(ServerTest, HandleStopCommand) {
    Logger::Init("test.log");
    std::string response = CommandHandler::HandleCommand("stop");
    EXPECT_EQ(response, "success stopped");
}

TEST(ServerTest, HandlePingCommand) {
    Logger::Init("test.log");
    std::string response = CommandHandler::HandleCommand("ping");
    EXPECT_EQ(response, "pong");
}

TEST(ServerTest, HandleInvalidCommand) {
    Logger::Init("test.log");
    std::string response = CommandHandler::HandleCommand("invalid");
    EXPECT_EQ(response, "error: unknown command");
}