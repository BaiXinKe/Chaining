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
    , doPending_ { false }
    , pendingTasks_ { std::make_unique<auxiliary::BlockQueue<PendingTask>>() }
{
    if (local_thread_id_ != std::thread::id {}) {
        ChainLogFatal("Only One Loop In a Thread\n");
        std::terminate();
    }
    local_thread_id_ = threadId_;

    wake_.reset(new WakeUp(this));
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
        doPendingTask();
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

    expirationTimers_.clear();
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
    assertInLoopThread();
    this->epoller_->update(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
    assertInLoopThread();
    this->epoller_->removeChannel(channel);
}

void EventLoop::queueInLoopThread(PendingTask task)
{
    if (isInLoopThread()) {
        task();
    } else {
        pendingTasks_->push(std::move(task));
        wake_->Signal();
    }
}

void EventLoop::runAfter(Time::TimerCallback timercb, std::chrono::milliseconds millseconds)
{
    this->queueInLoopThread([this, timercb, millseconds] {
        this->timers_->runAfter(std::move(timercb), millseconds);
    });
}

void EventLoop::runAfter(Time::TimerCallback timercb, double seconds)
{
    this->queueInLoopThread([this, timercb, seconds] {
        this->timers_->runAfter(std::move(timercb), seconds);
    });
}

void EventLoop::runUntil(Time::TimerCallback timercb, Time::Timestamp expire_time)
{
    this->queueInLoopThread([this, timercb, expire_time] {
        this->timers_->runUntil(std::move(timercb), expire_time);
    });
}

void EventLoop::runUntil(Time::TimerCallback timercb, time_t expire_time)
{
    this->queueInLoopThread([this, timercb, expire_time] {
        this->timers_->runUntil(std::move(timercb), expire_time);
    });
}

void EventLoop::runEvery(Time::TimerCallback timercb, std::chrono::milliseconds millseconds)
{
    this->queueInLoopThread([this, timercb, millseconds] {
        this->timers_->runEvery(std::move(timercb), millseconds);
    });
}

void EventLoop::runEvery(Time::TimerCallback timercb, double seconds)
{
    this->queueInLoopThread([this, timercb, seconds] {
        this->timers_->runEvery(std::move(timercb), seconds);
    });
}

void EventLoop::doPendingTask()
{
    PendingTask task_;
    while (!pendingTasks_->empty() && pendingTasks_->try_pop(task_)) {
        task_();
    }
}
