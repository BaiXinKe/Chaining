#ifndef CHAINING_MAIN_WAKEUP_HPP__
#define CHAINING_MAIN_WAKEUP_HPP__

#include "auxiliary/Nocopyable.hpp"

#include "main/Channel.hpp"

#include <memory>

namespace Chaining::net {

class EventLoop;

class WakeUp : auxiliary::nocopyable {
public:
    WakeUp(EventLoop* loop);

    void Signal();
    void handleRead();

    ~WakeUp();

private:
    int fd_;
    Channel channel_;
};

using WakeUPPtr = std::unique_ptr<WakeUp>;

}

#endif