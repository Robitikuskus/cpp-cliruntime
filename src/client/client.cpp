#include "client/client.hpp"
#include "common/logger.hpp"
#include "common/config.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcept>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

Client::Client() : socketFd(INVALID_SOCKET) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Logger::LogError("WSAStartup failed");
        throw std::runtime_error("WSAStartup failed");
    }
}

Client::~Client() {
    Disconnect();
    WSACleanup();
}

std::string Client::SendCommand(const std::string& command) {
    if (!Connect()) {
        throw std::runtime_error("Failed to connect to server: Service unavailable");
    }

    Logger::LogInfo(std::string("Sending command: ") + command);
    if (send(socketFd, command.c_str(), static_cast<int>(command.size()), 0) == SOCKET_ERROR) {
        Disconnect();
        throw std::runtime_error("Failed to send command");
    }

    char buffer[1024];
    int received = recv(socketFd, buffer, sizeof(buffer) - 1, 0);
    if (received <= 0) {
        Disconnect();
        throw std::runtime_error("No response from server");
    }

    buffer[received] = '\0';
    std::string response(buffer);
    Logger::LogInfo(std::string("Received response: ") + response);

    Disconnect();
    return response;
}

bool Client::Connect() {
    socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketFd == INVALID_SOCKET) {
        Logger::LogError("Failed to create socket");
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(Config::getInstance().getServerPort());
    inet_pton(AF_INET, Config::getInstance().getServerAddress().c_str(), &serverAddr.sin_addr);

    if (::connect(socketFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        Logger::LogError("Failed to connect to server");
        closesocket(socketFd);
        socketFd = INVALID_SOCKET;
        return false;
    }

    return true;
}

void Client::Disconnect() {
    if (socketFd != INVALID_SOCKET) {
        closesocket(socketFd);
        socketFd = INVALID_SOCKET;
    }
}