#ifndef CHAINING_BASE_ERRORMSG_HPP__
#define CHAINING_BASE_ERRORMSG_HPP__

#include <cerrno>
#include <cstring>
#include <string>
#include <string_view>

#include <iostream>

namespace Chaining {
namespace auxiliary {

    inline std::string errorMsg(std::string_view func, int err)
    {
        char buf[128] {};
        std::string msg { func };
        msg += ": ";

        char* ret = ::strerror_r(err, buf, sizeof(buf));
        if (ret != nullptr) {
            msg += std::string(ret, strlen(ret));
        } else {
            msg += "Unknown";
        }

        return msg;
    }

}
}
#define GetErrorMsg(err) \
    Chaining::auxiliary::errorMsg(__FUNCTION__, (err))

#endif