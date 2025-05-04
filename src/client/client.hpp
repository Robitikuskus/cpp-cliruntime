#pragma once

#include "socket_wrapper.hpp"
#include <string>

class Client {
public:
    Client();
    ~Client();
    std::string SendCommand(const std::string& command);

private:
    bool Connect();
    void Disconnect();
    SocketWrapper::socket_t socketFd;
};