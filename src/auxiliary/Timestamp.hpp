#ifndef CHAINING_AUXILIARY_TIMESTAMP_HPP__
#define CHAINING_AUXILIARY_TIMESTAMP_HPP__

#include <chrono>

namespace Chaining {
namespace Time {
    using Timestamp = std::chrono::system_clock::time_point;

    inline Timestamp now()
    {
        return std::chrono::system_clock::now();
    }
}
};

#endif