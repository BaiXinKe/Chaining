#ifndef CHAINING_MAIN_TIMERID_HPP__
#define CHAINING_MAIN_TIMERID_HPP__

#include "auxiliary/Nocopyable.hpp"

#include <atomic>
#include <cinttypes>

namespace Chaining::Time {

class TimerId : auxiliary::nocopyable {
public:
    TimerId();

    uint64_t getID() const { return uid_; }

private:
    static std::atomic<uint64_t> gId_;

    uint64_t uid_;
};

inline bool
operator<(const TimerId& left, const TimerId& right)
{
    return left.getID() < right.getID();
}

}

#endif