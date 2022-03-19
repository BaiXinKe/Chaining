#include <iostream>
#include <nlohmann/json.hpp>
#include <unistd.h>

#include "auxiliary/ReadSetting.hpp"

namespace aux = Chaining::auxiliary;

int main(int argc, char** argv)
{
    auto settings = aux::settings::parserSetting("../src/settings.json");
    initLogger();
}