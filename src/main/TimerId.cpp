#include "TimerId.hpp"

using namespace Chaining::Time;

std::atomic<uint64_t> TimerId::gId_ { 0 };

TimerId::TimerId()
    : uid_ { gId_.fetch_add(1) }
{
}