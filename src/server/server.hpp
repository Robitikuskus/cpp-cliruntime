#pragma once
#include <string>
#include <winsock2.h>

class Server {
public:
    Server();
    ~Server();
    void Run();

private:
    bool Init();
    void Cleanup();
    SOCKET socketFd;
};