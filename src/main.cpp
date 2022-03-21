#include <iostream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <unistd.h>

#include "auxiliary/ErrorMsg.hpp"
#include "auxiliary/InitLogger.hpp"
#include "auxiliary/Logger.hpp"
#include "auxiliary/ReadSetting.hpp"

namespace aux = Chaining::auxiliary;

int main(int argc, char** argv)
{
    ::system("pwd");
    auto settings = aux::settings::parserSetting("../src/settings.json");
    aux::InitLogger::initLogger(settings["logger"]);

    for (int i = 0; i < 100000; i++) {
        ChainLogInfo("Hello World");
    }
}