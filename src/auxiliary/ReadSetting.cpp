#include <cerrno>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string_view>

#include "ErrorMsg.hpp"
#include "ReadSetting.hpp"

using namespace Chaining::auxiliary;

nlohmann::json settings::parserSetting(std::string_view setting)
{
    nlohmann::json setting_json;

    std::ifstream setting_file { std::string(setting), std::ios::in };
    if (!setting_file.is_open()) {
        std::string msg { GetErrorMsg(errno) };
        throw std::runtime_error { msg };
    }

    setting_file >> setting_json;
    return setting_json;
}

size_t settings::GetUnitConversionValue(std::string_view capacity)
{
    static const std::unordered_map<std::string, size_t> unitMap {
        { "B", 1 }, { "KB", 1024 }, { "MB", 1024 * 1024 }, { "GB", 1024 * 1024 * 1024 }
    };

    if (capacity.empty())
        return unitMap.at("MB");

    char* numEnd { nullptr };
    long capacityNum { std::strtol(capacity.data(), &numEnd, 10) };
    if (capacityNum == -1) {
        std::string eMsg { GetErrorMsg(errno) };
        throw std::runtime_error { "GetUnitConversionValue Error: " + eMsg };
    }

    std::string unit_ { numEnd, capacity.size() - (numEnd - capacity.data()) };
    if (unit_.empty() || unitMap.find(unit_) == unitMap.end()) {
        unit_ = "MB";
    }

    size_t exposeFactor { unitMap.at(unit_) };

    return capacityNum * exposeFactor;
}