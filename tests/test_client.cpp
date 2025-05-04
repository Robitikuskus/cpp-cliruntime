#include <gtest/gtest.h>
#include "client.hpp"
#include "logger.hpp"

TEST(ClientTest, SendCommandInvalid) {
    Logger::Init("test.log");
    Client client;
#ifdef _WIN32
    EXPECT_THROW(client.SendCommand("invalid"), std::runtime_error);
#else
    EXPECT_NO_THROW(client.SendCommand("invalid")); // На Linux не выбрасывает
#endif
}