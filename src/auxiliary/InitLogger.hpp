#ifndef CHAINING_AUXILIARY_INIT_LOGGER_HPP__
#define CHAINING_AUXILIARY_INIT_LOGGER_HPP__

#include <nlohmann/json.hpp>

namespace Chaining {
namespace auxiliary {
    class InitLogger {
    public:
        static bool initLogger(nlohmann::json settings);

    private:
        static std::atomic<bool> inited_;
    };
}
}

#endif