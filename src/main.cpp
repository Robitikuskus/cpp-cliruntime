#include "common/logger.hpp"
#include "common/config.hpp"
#include "client/client.hpp"
#include "server/server.hpp"
#include <CLI/CLI.hpp>
#include <iostream>

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
        std::cout << "CLIRuntime v1.0.0, built on " << __DATE__ << " " << __TIME__ << std::endl;
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
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        FreeConsole();
        Server server;
        server.Run();
    } catch (const std::exception& e) {
        Logger::LogError(std::string("Server failed: ") + e.what());
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}