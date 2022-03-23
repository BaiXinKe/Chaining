#include "main/EventLoop.hpp"
#include "main/Channel.hpp"
#include "main/Epoller.hpp"

#include "auxiliary/Logger.hpp"

#include <cassert>

using namespace Chaining::net;

thread_local std::thread::id local_thread_id_ {};

EventLoop::EventLoop()
    : threadId_ { std::this_thread::get_id() }
    , epoller_ { std::make_unique<Epoller>(this) }
    , timers_ { std::make_unique<Time::TimerQueue>() }
    , stop_ { false }
    , wake_ { std::make_unique<WakeUp>(this) }
{
    if (local_thread_id_ != std::thread::id {}) {
        ChainLogFatal("Only One Loop In a Thread\n");
        std::terminate();
    }

    local_thread_id_ = threadId_;
}

void EventLoop::loop()
{
    while (!stop_) {
        activatedChannels_.clear();

        Time::Timestamp nextExp { timers_->getNextExpiredTime() };

        epoller_->wait(&activatedChannels_, nextExp);

        for (auto& channel : activatedChannels_) {
            channel->handleEvent();
        }

        execExpiredTimesTask();
    }
}

void EventLoop::execExpiredTimesTask()
{
    timers_->getExpiredTimes(&expirationTimers_);
    for (auto& timer_ : expirationTimers_) {
        timer_->run();
        if (timer_->isRepeat()) {
            timer_->reset();
            timers_->addTimer(std::move(timer_));
        }
    }
}

bool EventLoop::isInLoopThread()
{
    return threadId_ == local_thread_id_;
}

void EventLoop::assertInLoopThread()
{
    assert(isInLoopThread());
}

void EventLoop::update(Channel* channel)
{
    assert(isInLoopThread());
    this->epoller_->update(channel);
}