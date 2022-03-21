#include "InitLogger.hpp"
#include "ReadSetting.hpp"

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <vector>

using namespace Chaining::auxiliary;

static constexpr size_t DEFAULT_BUFFER_SIZE { 8192 };

std::atomic<bool> InitLogger::inited_ { false };

bool InitLogger::initLogger(nlohmann::json log_settings)
{
    if (inited_.load())
        return true;

    // stdout/stderr log settings --------------------------------------------------------------
    size_t bufferSize {};
    if (log_settings.count("async_buffer_size"))
        bufferSize = log_settings["async_buffer_size"];
    else
        bufferSize = DEFAULT_BUFFER_SIZE;

    spdlog::init_thread_pool(bufferSize, 1);
    std::vector<spdlog::sink_ptr> sinks_ {};

    if (log_settings.count("out2std") && log_settings["out2std"]) {
        sinks_.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    }

    // rotate logger settings -------------------------------------------------------------------
    if (log_settings.count("only2std") && !log_settings["only2std"]) {

        std::string base_log_name { "logs/log.txt" };
        if (log_settings.count("base_log_name") != 0)
            base_log_name = log_settings["base_log_name"];

        size_t maxSize { 1024 * 1024 * 1024 }; // default 1G
        if (log_settings.count("max_size")) {
            maxSize = settings::GetUnitConversionValue(std::string(log_settings["max_size"]));
        }

        size_t maxFiles { 1 };
        if (log_settings.count("max_files") != 0) {
            maxFiles = log_settings["max_files"];
        }

        sinks_.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(base_log_name, maxSize, maxFiles));

        std::chrono::seconds flush_every_s { 1 };
        if (log_settings.count("flush_every_ms")) {
            flush_every_s = std::chrono::seconds(log_settings["flush_every_s"]);
        }
        spdlog::flush_every(flush_every_s);
    }

    spdlog::level::level_enum level { spdlog::level::level_enum::debug };

    if (log_settings.count("level")) {
        const std::unordered_map<std::string, spdlog::level::level_enum> level_trans {
            { "trace", spdlog::level::level_enum::trace }, { "debug", spdlog::level::level_enum::debug },
            { "info", spdlog::level::level_enum::info }, { "warning", spdlog::level::level_enum::warn },
            { "error", spdlog::level::level_enum::err }, { "fatal", spdlog::level::level_enum::critical }
        };

        level = level_trans.at(std::string(log_settings["level"]));
    }

    // setting logger level -------------------------------------------------------------------------
    for (auto& logger : sinks_) {
        logger->set_level(level);
    }

    auto logger = std::make_shared<spdlog::async_logger>("Chaining", sinks_.begin(), sinks_.end(),
        spdlog::thread_pool(), spdlog::async_overflow_policy::overrun_oldest);

    spdlog::set_default_logger(logger);

    spdlog::info("test");

    return true;
}