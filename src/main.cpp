#include <iostream>
#include <nlohmann/json.hpp>
#include <unistd.h>

#include "auxiliary/InitLogger.hpp"
#include "auxiliary/ReadSetting.hpp"

namespace aux = Chaining::auxiliary;

int main(int argc, char** argv)
{
    ::system("pwd");
    auto settings = aux::settings::parserSetting("./src/settings.json");
    aux::InitLogger::initLogger(settings["logger"]);
}