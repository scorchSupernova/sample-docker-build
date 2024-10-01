#include "stdafx.h"
#include "logger.h"

std::shared_ptr<spdlog::logger> Logger::s_logger;

std::shared_ptr<spdlog::logger> Logger::get()
{
    std::ifstream config_file("config.json");
    nlohmann::json config;
    config_file >> config;

    std::time_t now = std::time(nullptr);
    std::tm tm = *std::localtime(&now);

    char filename[100];
    std::strftime(filename, sizeof(filename), "log/mt5-ms-%Y-%m-%d.log", &tm);
    std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("logger", filename);

    // Set the log level based on the values in the config file
    if (config["logger"]["debug"]) {
        logger->set_level(spdlog::level::debug);
        logger->flush_on(spdlog::level::debug);
    }
    else if (config["logger"]["info"]) {
        logger->set_level(spdlog::level::info);
        logger->flush_on(spdlog::level::info);
    }
    else if (config["logger"]["error"]) {
        logger->set_level(spdlog::level::err);
        logger->flush_on(spdlog::level::err);
    }

    return logger;

}
