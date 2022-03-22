#include "Timer.hpp"
#include <cassert>

using namespace Chaining::Time;

Timer::Timer(TimerCallback timercb, ExpireTime expTime)
    : id_ {}
    , timercb_ { std::move(timercb) }
    , isRepeat_ { false }
    , expTime_ { expTime }
    , interval_ { IntervalUnit {} }
    , canceled_ { false }
{
}

Timer::Timer(TimerCallback timercb, IntervalUnit interval)
    : id_ {}
    , timercb_ { std::move(timercb) }
    , isRepeat_ { true }
    , expTime_ { now() + interval_ }
    , interval_ { interval_ }
    , canceled_ { false }
{
    assert(interval_ > IntervalUnit {});
}

Timer::Timer(TimerCallback timercb, ExpireTime expTime, IntervalUnit interval)
    : id_ {}
    , timercb_ { std::move(timercb) }
    , isRepeat_ { true }
    , expTime_ { expTime }
    , interval_ { interval }
    , canceled_ { false }
{
    assert(interval_ > IntervalUnit {});
}

bool Timer::isExpired() const
{
    return expTime_ >= now();
}

void Timer::reset()
{
    if (isRepeat() && !isCanceled()) {
        expTime_ = now() + interval_;
    } else {
        expTime_ = Timestamp {};
    }
}

void Timer::run()
{
    if (this->timercb_)
        timercb_();
}