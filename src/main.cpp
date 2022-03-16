#include <fcntl.h>
#include <liburing.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <fstream>
#include <vector>

#include "base/LoggerUtility.hpp"

int main(int argc, char* argv[])
{
    using namespace Chaining::base;

    FileUtility utility("./log/xLogger", 1, 2, 1024 * 1024 * 512);

    std::vector<char> Mbytes(1024 * 1024, 'x');
    for (int i = 0; i < 1024; i++)
        utility.append(Mbytes.data(), Mbytes.size());
}