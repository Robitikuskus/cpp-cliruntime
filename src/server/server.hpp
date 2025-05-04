#pragma once
#include "socket_wrapper.hpp"

class Server {
public:
    Server();
    void Run();

private:
    bool Init();
    SocketWrapper::socket_t socketFd;
};