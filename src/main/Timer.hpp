#ifndef CHAINING_MAIN_TIMER_HPP__
#define CHAINING_MAIN_TIMER_HPP__

#include "auxiliary/Nocopyable.hpp"
#include "auxiliary/Timestamp.hpp"

#include "main/Callback.hpp"
#include "main/TimerId.hpp"

namespace Chaining::Time {

class Timer : auxiliary::nocopyable {
public:
    using IntervalUnit = std::chrono::milliseconds;
    using ExpireTime = Timestamp;
    Timer() = default;

    Timer(TimerCallback timercb, ExpireTime expTime);
    Timer(TimerCallback timercb, IntervalUnit interval);

    Timer(TimerCallback timercb, ExpireTime expTime, IntervalUnit interval);

    bool isRepeat() const { return isRepeat_; }
    ExpireTime expiration() const { return expTime_; }
    bool isExpired() const;

    void reset();

    void cancel() { canceled_ = true; }
    bool isCanceled() const { return canceled_; }

    void run();

    ~Timer() = default;

    friend bool operator<(const Timer& left, const Timer& right);

private:
    TimerId id_;
    TimerCallback timercb_;
    bool isRepeat_;
    Timestamp expTime_;
    IntervalUnit interval_;

    bool canceled_;
};

inline bool
operator<(const Timer& left, const Timer& right)
{
    return left.expTime_ < right.expTime_
        || (left.expTime_ == right.expTime_ && left.id_ < right.id_);
}

}

#endif