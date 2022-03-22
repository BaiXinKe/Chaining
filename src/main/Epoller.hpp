#ifndef CHAINING_MAIN_EPOLLER_HPP__
#define CHAINING_MAIN_EPOLLER_HPP__

#include "auxiliary/Nocopyable.hpp"
#include <vector>

#include <memory>

namespace Chaining::net {

class EventLoop;

class Epoller : auxiliary::nocopyable {
public:
    static constexpr size_t INIT_EVENT_LIST_SIZE { 64 };

    explicit Epoller(EventLoop* loop, size_t eventListSize = INIT_EVENT_LIST_SIZE);

    ssize_t wait();

private:
    EventLoop* loop_;

    int epollfd_;
};

using EpollPtr = std::unique_ptr<Epoller>;

};

#endif