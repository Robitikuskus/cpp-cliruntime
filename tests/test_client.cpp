#include <gtest/gtest.h>
#include "client/client.hpp"
#include "common/logger.hpp"

TEST(ClientTest, SendCommandInvalid) {
    Logger::Init("test.log");
    Client client;
    EXPECT_THROW(client.SendCommand("invalid"), std::runtime_error);
}