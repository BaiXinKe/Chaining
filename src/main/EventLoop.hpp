#ifndef CHAINING_MAIN_EVENTLOOP_HPP__
#define CHAINING_MAIN_EVENTLOOP_HPP__

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

private:
    void execExpiredTimesTask();

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
};

}

#endif