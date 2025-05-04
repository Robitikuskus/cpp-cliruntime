#pragma once
#include <string>

class CommandHandler {
public:
    static std::string HandleCommand(const std::string& command);

private:
    static std::string HandleListen(const std::string& directory);
    static std::string HandleStatus();
};