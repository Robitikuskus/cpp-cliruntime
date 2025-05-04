#include "config.hpp"

Config::Config() : serverPort(12345), serverAddress("127.0.0.1") {}

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

uint16_t Config::getServerPort() const {
    return serverPort;
}

void Config::setServerPort(uint16_t port) {
    serverPort = port;
}

std::string Config::getServerAddress() const {
    return serverAddress;
}