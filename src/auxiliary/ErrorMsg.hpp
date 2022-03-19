#ifndef CHAINING_BASE_ERRORMSG_HPP__
#define CHAINING_BASE_ERRORMSG_HPP__

#include <cstring>
#include <string>
#include <string_view>

namespace Chaining {
namespace auxiliary {

    inline std::string errorMsg(std::string_view func, int err)
    {
        char buf[128] {};
        std::string msg { func };
        msg += ": ";

        if (strerror_r(err, buf, sizeof(buf)) != nullptr) {
            msg += std::string(buf, strlen(buf));
        } else {
            msg += "Unknown";
        }

        return msg;
    }

#define GetErrorMsg() errorMsg(__FUNCTION__, (errno))

}
}

#endif