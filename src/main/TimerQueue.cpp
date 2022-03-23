#include "auxiliary/Logger.hpp"

#include "main/TimerQueue.hpp"

#include <cassert>

using namespace Chaining;

static constexpr int MILL_PER_SECONDS { 1000 };

bool Time::TimerQueue::TimerPtrLess::operator()(const TimerPtr& left, const TimerPtr& right) const
{
    return (*left) < (*right);
}

void Time::TimerQueue::runAfter(TimerCallback timercb, std::chrono::milliseconds millsconds)
{
    Timestamp ts = Now() + millsconds;
    runUntil(std::move(timercb), std::move(ts));
}

void Time::TimerQueue::runAfter(TimerCallback timercb, double seconds)
{
    uint64_t millseconds = seconds * MILL_PER_SECONDS;
    Timestamp ts = Now() + std::chrono::milliseconds(millseconds);
    runUntil(std::move(timercb), std::move(ts));
}

void Time::TimerQueue::runUntil(TimerCallback timercb, Timestamp expire_time)
{
    TimerPtr timer { std::make_unique<Timer>(std::move(timercb), expire_time) };

    assert(!timer->isRepeat());

    timers_.insert(std::move(timer));
}

void Time::TimerQueue::runUntil(TimerCallback timercb, time_t expire_time)
{
    Timestamp ts { std::chrono::seconds(expire_time) };
    runUntil(std::move(timercb), ts);
}

void Time::TimerQueue::runEvery(TimerCallback Timercb, std::chrono::milliseconds millseconds)
{
    Timestamp expire_time = Now() + millseconds;
    TimerPtr timer { std::make_unique<Timer>(std::move(Timercb), expire_time, millseconds) };

    assert(timer->isRepeat());

    ChainLogInfo("New clock task start {}", Timercb.target_type().name());
    timers_.insert(std::move(timer));
}

void Time::TimerQueue::runEvery(TimerCallback timercb, double seconds)
{
    std::chrono::milliseconds ms {
        std::chrono::milliseconds(static_cast<uint64_t>(seconds * MILL_PER_SECONDS))
    };
    runEvery(std::move(timercb), ms);
}

Time::Timestamp Time::TimerQueue::getNextExpiredTime() const
{
    if (timers_.empty()) {
        return Timestamp {};
    }
    auto next = timers_.begin();
    return (*next)->expiration();
}

void Time::TimerQueue::getExpiredTimes(std::vector<TimerPtr>* expired_times)
{
    Timestamp curr { Now() };

    while (!timers_.empty() && (*timers_.begin())->expiration() <= curr) {
        auto timer_node = timers_.extract(timers_.begin());
        expired_times->push_back(std::move(timer_node.value()));
    }
}

void Time::TimerQueue::addTimer(TimerPtr timer)
{
    assert(timer != nullptr);
    assert(timers_.find(timer) == timers_.end());

    timers_.insert(std::move(timer));
}