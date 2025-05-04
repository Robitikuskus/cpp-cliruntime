#pragma once
#include <string>
#include <winsock2.h>

class Client {
public:
    Client();
    ~Client();
    std::string SendCommand(const std::string& command);

private:
    bool Connect();
    void Disconnect();
    SOCKET socketFd;
};