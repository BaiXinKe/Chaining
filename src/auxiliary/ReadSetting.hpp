#ifndef CHAINING_BASE_READSETTING_HPP__
#define CHAINING_BASE_READSETTING_HPP__

#include <string_view>

#include <nlohmann/json.hpp>
#include <unordered_map>

namespace Chaining {
namespace auxiliary {

    namespace settings {
        nlohmann::json parserSetting(std::string_view setting);
        size_t GetUnitConversionValue(std::string_view);
    }

}
}

#endif