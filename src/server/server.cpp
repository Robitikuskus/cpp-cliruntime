#include "server.hpp"
#include "command_handler.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "socket_wrapper.hpp"
#include <stdexcept>
#include <chrono>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

Server::Server()
    : socketFd(SocketWrapper::INVALID_SOCKET_VALUE)
{
}

void Server::Run() {
    if (!Init()) {
        throw std::runtime_error("Failed to initialize server");
    }

    Logger::LogInfo(std::string("Server started on port ") + std::to_string(Config::getInstance().getServerPort()));

    char buffer[1024];
    sockaddr_in clientAddr;
    
    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        try {
            int received = SocketWrapper::ReceiveFromSocket(socketFd, buffer, sizeof(buffer) - 1, clientAddr);
            buffer[received] = '\0';
            std::string command(buffer);
            Logger::LogInfo(std::string("Received command: ") + command);

            // Log uptime
            auto now = std::chrono::steady_clock::now();
            auto uptime = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
            Logger::LogInfo(std::string("System uptime: ") + std::to_string(uptime) + " seconds");

            std::string response = CommandHandler::HandleCommand(command);

            SocketWrapper::SendToSocket(socketFd, response.c_str(), response.size(), clientAddr);

            if (command == "stop") {
                Logger::LogInfo("Stopping server");
                break;
            }
        } catch (const std::exception& e) {
            Logger::LogError("Failed to process data: " + std::string(e.what()));
            continue;
        }
    }
}

bool Server::Init() {
    try {
        socketFd = SocketWrapper::CreateSocket();
        SocketWrapper::BindSocket(socketFd, "0.0.0.0", Config::getInstance().getServerPort());
        return true;
    } catch (const std::exception& e) {
        Logger::LogError("Server initialization failed: " + std::string(e.what()));
        socketFd = SocketWrapper::INVALID_SOCKET_VALUE;
        return false;
    }
}