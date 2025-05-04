#pragma once

#include <string>
#include <stdexcept>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>
#endif

namespace SocketWrapper {

#ifdef _WIN32
    using socket_t = SOCKET;
    constexpr socket_t INVALID_SOCKET_VALUE = INVALID_SOCKET;
#else
    using socket_t = int;
    constexpr socket_t INVALID_SOCKET_VALUE = -1;
#endif

#ifdef _WIN32
    struct WSAInitializer {
        WSAInitializer() {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                throw std::runtime_error("WSAStartup failed: " + std::to_string(WSAGetLastError()));
            }
        }
        ~WSAInitializer() {
            WSACleanup();
        }
    };
    static WSAInitializer wsa_initializer;
#endif

inline socket_t CreateSocket() {
    socket_t sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET_VALUE) {
        throw std::runtime_error("Socket creation failed: " + std::to_string(
#ifdef _WIN32
            WSAGetLastError()
#else
            errno
#endif
        ));
    }
    return sock;
}

inline void CloseSocket(socket_t sock) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}

inline void BindSocket(socket_t sock, const std::string& ip, int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        throw std::runtime_error("Bind failed: " + std::to_string(
#ifdef _WIN32
            WSAGetLastError()
#else
            errno
#endif
        ));
    }
}

inline void ConnectSocket(socket_t sock, const std::string& ip, int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        throw std::runtime_error("Connect failed: " + std::to_string(
#ifdef _WIN32
            WSAGetLastError()
#else
            errno
#endif
        ));
    }
}

inline int SendSocket(socket_t sock, const char* data, size_t size) {
    int bytes_sent = send(sock, data, size, 0);
    if (bytes_sent == -1) {
        throw std::runtime_error("Send failed: " + std::to_string(
#ifdef _WIN32
            WSAGetLastError()
#else
            errno
#endif
        ));
    }
    return bytes_sent;
}

inline int ReceiveSocket(socket_t sock, char* buffer, size_t size) {
    int bytes_received = recv(sock, buffer, size, 0);
    if (bytes_received == -1) {
        throw std::runtime_error("Recv failed: " + std::to_string(
#ifdef _WIN32
            WSAGetLastError()
#else
            errno
#endif
        ));
    }
    return bytes_received;
}

inline int SendToSocket(socket_t sock, const char* data, size_t size, const sockaddr_in& addr) {
    int addr_len = sizeof(addr);
    int bytes_sent = sendto(sock, data, size, 0, (struct sockaddr*)&addr, addr_len);
    if (bytes_sent == -1) {
        throw std::runtime_error("Sendto failed: " + std::to_string(
#ifdef _WIN32
            WSAGetLastError()
#else
            errno
#endif
        ));
    }
    return bytes_sent;
}

inline int ReceiveFromSocket(socket_t sock, char* buffer, size_t size, sockaddr_in& addr) {
    socklen_t addr_len = sizeof(addr);
    int bytes_received = recvfrom(sock, buffer, size, 0, (struct sockaddr*)&addr, &addr_len);
    if (bytes_received == -1) {
        throw std::runtime_error("Recvfrom failed: " + std::to_string(
#ifdef _WIN32
            WSAGetLastError()
#else
            errno
#endif
        ));
    }
    return bytes_received;
}

} // namespace SocketWrapper