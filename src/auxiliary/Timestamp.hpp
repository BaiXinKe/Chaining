#ifndef CHAINING_AUXILIARY_TIMESTAMP_HPP__
#define CHAINING_AUXILIARY_TIMESTAMP_HPP__

#include <chrono>

namespace Chaining {
namespace Time {
    using Timestamp = std::chrono::system_clock::time_point;

    inline Timestamp Now()
    {
        return std::chrono::system_clock::now();
    }

    inline int64_t GetMillsecondsFromNow(Timestamp exptime)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(exptime - Now()).count();
    }
}
};

#endif