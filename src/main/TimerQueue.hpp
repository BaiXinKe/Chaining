#ifndef CHAINING_MAIN_TIMERQUEUE_HPP__
#define CHAINING_MAIN_TIMERQUEUE_HPP__

#include "auxiliary/Nocopyable.hpp"
#include "auxiliary/Timestamp.hpp"

#include "main/Callback.hpp"
#include "main/Timer.hpp"

#include <memory>
#include <set>

namespace Chaining::Time {

class TimerQueue : auxiliary::nocopyable {
public:
    TimerQueue() = default;

    void runAfter(TimerCallback timercb, std::chrono::milliseconds millseconds);
    void runAfter(TimerCallback timercb, double seconds);

    void runUntil(TimerCallback timercb, Timestamp expire_time);
    void runUntil(TimerCallback timercb, time_t expire_time);

    void runEvery(TimerCallback timercb, std::chrono::milliseconds millseconds);
    void runEvery(TimerCallback timercb, double seconds);

    void addTimer(TimerPtr timer);

    Timestamp getNextExpiredTime() const;

    void getExpiredTimes(std::vector<TimerPtr>* expired_times);

private:
    struct TimerPtrLess {
    public:
        bool operator()(const TimerPtr& left, const TimerPtr& right) const;
    };

private:
    using TimerMultiSet = std::multiset<TimerPtr, TimerPtrLess>;
    TimerMultiSet timers_;
};

using TimerQueuePtr = std::unique_ptr<TimerQueue>;

}

#endif