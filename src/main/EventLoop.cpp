#include "main/EventLoop.hpp"
#include "main/Epoller.hpp"

#include "auxiliary/Logger.hpp"

#include <cassert>

using namespace Chaining::net;

thread_local std::thread::id local_thread_id_ {};

EventLoop::EventLoop()
    : threadId_ { std::this_thread::get_id() }
    , epoller_ { std::make_unique<Epoller>(this) }
    , stop_ { false }
{
    if (local_thread_id_ != std::thread::id {}) {
        ChainLogFatal("Only One Loop In a Thread: Thread Id {}\n", local_thread_id_);
        std::terminate();
    }

    local_thread_id_ = threadId_;
}

void EventLoop::loop()
{
    while (!stop_) {
        epoller_->wait();
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