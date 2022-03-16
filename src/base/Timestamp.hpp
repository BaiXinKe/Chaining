#ifndef CHAINING_BASE_TIMESTAMP_HPP__
#define CHAINING_BASE_TIMESTAMP_HPP__

#include <chrono>

namespace Chaining {
namespace base {
    namespace time {
        using Timestamp = std::chrono::steady_clock::time_point;

        inline Timestamp currentTime()
        {
            return std::chrono::steady_clock::now();
        }
        inline bool isPassedTime(Timestamp timestamp)
        {
            return timestamp < currentTime();
        }
    } // namespace time
} // namespace base
} // namespace Chaining

#endif