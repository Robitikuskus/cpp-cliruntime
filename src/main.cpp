#include "logger.hpp"
#include "config.hpp"
#include "client.hpp"
#include "server.hpp"
#include <CLI/CLI.hpp>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

int main(int argc, char* argv[]) {
    Logger::Init("cliruntime.log");

    CLI::App app{"CLIRuntime"};
    bool version = false;
    std::string command;
    app.add_flag("-v,--version", version, "Print version string");
    app.add_option("-c,--cmd", command, "Send command to service");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        Logger::LogError(std::string("Failed to parse command line: ") + e.what());
        std::cerr << "Error: " << e.what() << std::endl;
        return app.exit(e);
    }

    if (version) {
        std::cout << "CLIRuntime v1.1.0, built on " << __DATE__ << " " << __TIME__ << std::endl;
        return 0;
    }

    if (!command.empty()) {
        Client client;
        try {
            std::string response = client.SendCommand(command);
            std::cout << response << std::endl;
            return 0;
        } catch (const std::exception& e) {
            Logger::LogError(std::string("Client failed: ") + e.what());
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

    // Run as service
    try {
#ifdef _WIN32
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    FreeConsole();
#else
    pid_t pid = fork();
    if (pid < 0) {
        throw std::runtime_error("fork() failed:\n" + std::string(strerror(errno)));
    }
    if (pid > 0) {
        exit(0);
    }
    umask(0);
    if (setsid() < 0) {
        throw std::runtime_error("setsid() failed:\n" + std::string(strerror(errno)));
    }
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        throw std::runtime_error("Failed to open /dev/null:\n" + std::string(strerror(errno)));
    }
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);
#endif

        Server server;
        server.Run();
    } catch (const std::exception& e) {
        Logger::LogError(std::string("Server failed: ") + e.what());
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
