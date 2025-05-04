#include "client.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "socket_wrapper.hpp"
#include <stdexcept>
#include <iostream>

Client::Client() : socketFd(SocketWrapper::INVALID_SOCKET_VALUE) {
}

Client::~Client() {
    Disconnect();
}

std::string Client::SendCommand(const std::string& command) {
    if (!Connect()) {
        throw std::runtime_error("Failed to connect to server: Service unavailable");
    }

    try {
        Logger::LogInfo(std::string("Sending command: ") + command);
        SocketWrapper::SendSocket(socketFd, command.c_str(), command.size());

        char buffer[1024] = {0};
        int received = SocketWrapper::ReceiveSocket(socketFd, buffer, sizeof(buffer) - 1);
        if (received <= 0) {
            throw std::runtime_error("No response from server");
        }

        buffer[received] = '\0';
        std::string response(buffer);
        Logger::LogInfo(std::string("Received response: ") + response);

        Disconnect();
        return response;
    } catch (const std::exception& e) {
        Disconnect();
        throw std::runtime_error(std::string("Failed to send command or receive response: ") + e.what());
    }
}

bool Client::Connect() {
    try {
        socketFd = SocketWrapper::CreateSocket();
        SocketWrapper::ConnectSocket(socketFd, Config::getInstance().getServerAddress(), Config::getInstance().getServerPort());
        return true;
    } catch (const std::exception& e) {
        Logger::LogError("Failed to connect to server: " + std::string(e.what()));
        if (socketFd != SocketWrapper::INVALID_SOCKET_VALUE) {
            SocketWrapper::CloseSocket(socketFd);
            socketFd = SocketWrapper::INVALID_SOCKET_VALUE;
        }
        return false;
    }
}

void Client::Disconnect() {
    if (socketFd != SocketWrapper::INVALID_SOCKET_VALUE) {
        SocketWrapper::CloseSocket(socketFd);
        socketFd = SocketWrapper::INVALID_SOCKET_VALUE;
    }
}