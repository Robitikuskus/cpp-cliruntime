#include "command_handler.hpp"
#include "common/logger.hpp"
#include <filesystem>
#include <sstream>
#include <chrono>

std::string CommandHandler::HandleCommand(const std::string& command) {
    if (command == "stop") {
        return "success stopped";
    } else if (command.find("listen ") == 0) {
        std::string directory = command.substr(7);
        return HandleListen(directory);
    } else if (command == "status") {
        return HandleStatus();
    } else if (command == "ping") {
        return "pong";
    } else {
        std::string logMsg = std::string("Unknown command: ") + command;
        Logger::LogWarning(logMsg);
        return "error: unknown command";
    }
}

std::string CommandHandler::HandleListen(const std::string& directory) {
    std::filesystem::path dirPath(directory);
    if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath)) {
        std::string logMsg = std::string("Failed to open directory: ") + directory;
        Logger::LogError(logMsg);
        return "error: cannot open directory";
    }

    std::stringstream ss;
    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (!entry.is_directory()) {
            ss << entry.path().filename().string() << "\n";
        }
    }

    std::string result = ss.str();
    if (result.empty()) {
        return "empty directory";
    }
    return result;
}

std::string CommandHandler::HandleStatus() {
    auto now = std::chrono::steady_clock::now();
    static auto startTime = now;
    auto uptime = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
    std::stringstream ss;
    ss << "uptime: " << uptime << " seconds";
    return ss.str();
}