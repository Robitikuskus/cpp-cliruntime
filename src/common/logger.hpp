#pragma once

#include <spdlog/spdlog.h>
#include <string>

class Logger {
public:
    static void Init(const std::string& log_file);
    static void LogInfo(const std::string& message);
    static void LogError(const std::string& message);
    static void LogWarning(const std::string& message);
private:
    static std::shared_ptr<spdlog::logger> logger;
};