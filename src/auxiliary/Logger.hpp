#ifndef CHAINING_AUXILIARY_LOGGER_HPP__
#define CHAINING_AUXILIARY_LOGGER_HPP__

#include <spdlog/spdlog.h>

namespace Chaining {
namespace auxiliary {

#define ChainLogTrace(...) SPDLOG_TRACE(__VA_ARGS__)
#define ChainLogDebug(...) SPDLOG_DEBUG(__VA_ARGS__)
#define ChainLogInfo(...) SPDLOG_INFO(__VA_ARGS__)
#define ChainLogWarning(...) SPDLOG_WARN(__VA_ARGS__)
#define ChainLogError(...) SPDLOG_ERROR(__VA_ARGS__)

#define ChainLogFatal(...) SPDLOG_CRITICAL(__VA_ARGS__)

}
}

#endif