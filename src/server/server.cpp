#include "server/server.hpp"
#include "server/command_handler.hpp"
#include "common/logger.hpp"
#include "common/config.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcept>
#include <chrono>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")

Server::Server() : socketFd(INVALID_SOCKET) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Logger::LogError("WSAStartup failed");
        throw std::runtime_error("WSAStartup failed");
    }
}

Server::~Server() {
    Cleanup();
    WSACleanup();
}

void Server::Run() {
    if (!Init()) {
        throw std::runtime_error("Failed to initialize server");
    }

    Logger::LogInfo(std::string("Server started on port ") + std::to_string(Config::getInstance().getServerPort()));

    char buffer[1024];
    sockaddr_in clientAddr;
    int clientLen = sizeof(clientAddr);

    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        int received = recvfrom(socketFd, buffer, sizeof(buffer) - 1, 0,
                                (sockaddr*)&clientAddr, &clientLen);
        if (received == SOCKET_ERROR) {
            Logger::LogError("Failed to receive data");
            continue;
        }

        buffer[received] = '\0';
        std::string command(buffer);
        Logger::LogInfo(std::string("Received command: ") + command);

        // Log uptime
        auto now = std::chrono::steady_clock::now();
        auto uptime = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        Logger::LogInfo(std::string("System uptime: ") + std::to_string(uptime) + " seconds");

        std::string response = CommandHandler::HandleCommand(command);

        sendto(socketFd, response.c_str(), static_cast<int>(response.size()), 0,
               (sockaddr*)&clientAddr, clientLen);

        if (command == "stop") {
            Logger::LogInfo("Stopping server");
            break;
        }
    }

    Cleanup();
}

bool Server::Init() {
    socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketFd == INVALID_SOCKET) {
        Logger::LogError("Failed to create socket");
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(Config::getInstance().getServerPort());
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socketFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        Logger::LogError("Failed to bind socket");
        closesocket(socketFd);
        socketFd = INVALID_SOCKET;
        return false;
    }

    return true;
}

void Server::Cleanup() {
    if (socketFd != INVALID_SOCKET) {
        closesocket(socketFd);
        socketFd = INVALID_SOCKET;
    }
}