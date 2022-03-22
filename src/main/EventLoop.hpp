#ifndef CHAINING_MAIN_EVENTLOOP_HPP__
#define CHAINING_MAIN_EVENTLOOP_HPP__

#include "auxiliary/Nocopyable.hpp"

#include "main/Epoller.hpp"

#include <atomic>
#include <memory>
#include <thread>

namespace Chaining::net {

class EventLoop : auxiliary::nocopyable {
public:
    EventLoop();

    void loop();

    void assertInLoopThread();
    bool isInLoopThread();

private:
    std::thread::id threadId_;
    EpollPtr epoller_;
    std::atomic<bool> stop_;
};

}

#endif