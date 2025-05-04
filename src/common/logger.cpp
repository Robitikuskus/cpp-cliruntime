#include "logger.hpp"
#include <spdlog/sinks/rotating_file_sink.h>
#include <filesystem>

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

void Logger::Init(const std::string& log_file) {
    try {
        // Check if logger already exists
        if (logger) {
            return; // Logger already initialized, no need to create a new one
        }
        logger = spdlog::rotating_logger_mt("cliruntime", log_file, 5 * 1024 * 1024, 3);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
        logger->set_level(spdlog::level::trace);
        spdlog::set_default_logger(logger);
    } catch (const spdlog::spdlog_ex& ex) {
        throw std::runtime_error("Logger initialization failed: " + std::string(ex.what()));
    }
}

void Logger::LogInfo(const std::string& message) {
    if (logger) {
        logger->info(message);
    }
}

void Logger::LogError(const std::string& message) {
    if (logger) {
        logger->error(message);
    }
}

void Logger::LogWarning(const std::string& message) {
    if (logger) {
        logger->warn(message);
    }
}