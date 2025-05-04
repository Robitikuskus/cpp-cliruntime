#pragma once
#include <cstdint>
#include <string>

class Config {
public:
    static Config& getInstance();
    uint16_t getServerPort() const;
    void setServerPort(uint16_t port);
    std::string getServerAddress() const;

private:
    Config();
    uint16_t serverPort;
    std::string serverAddress;
};