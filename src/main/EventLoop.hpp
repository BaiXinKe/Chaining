#ifndef CHAINING_MAIN_EVENTLOOP_HPP__
#define CHAINING_MAIN_EVENTLOOP_HPP__

#include "auxiliary/BlockQueue.hpp"
#include "auxiliary/Nocopyable.hpp"

#include "main/Epoller.hpp"
#include "main/TimerQueue.hpp"
#include "main/WakeUp.hpp"

#include <atomic>
#include <memory>
#include <thread>

namespace Chaining::net {

class Channel;

class EventLoop : auxiliary::nocopyable {
public:
    EventLoop();

    void loop();

    void assertInLoopThread();
    bool isInLoopThread();

    void update(Channel* channel);
    void removeChannel(Channel* channel);

    void queueInLoopThread(PendingTask task);

    void runAfter(Time::TimerCallback timercb, std::chrono::milliseconds millseconds);
    void runAfter(Time::TimerCallback timercb, double seconds);

    void runUntil(Time::TimerCallback timercb, Time::Timestamp expire_time);
    void runUntil(Time::TimerCallback timercb, time_t expire_time);

    void runEvery(Time::TimerCallback timercb, std::chrono::milliseconds millseconds);
    void runEvery(Time::TimerCallback timercb, double seconds);

private:
    void execExpiredTimesTask();
    void doPendingTask();

private:
    using ActivatedChannels = std::vector<Channel*>;
    using ExpirationTimers = std::vector<Time::TimerPtr>;

    std::thread::id threadId_;
    EpollPtr epoller_;
    Time::TimerQueuePtr timers_;

    std::atomic<bool> stop_;

    WakeUPPtr wake_;

    ActivatedChannels activatedChannels_;
    ExpirationTimers expirationTimers_;

    bool doPending_;
    auxiliary::BlockQueuePtr<PendingTask> pendingTasks_;
};

}

#endif