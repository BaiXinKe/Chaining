#include "auxiliary/Logger.hpp"
#include "auxiliary/Timestamp.hpp"

#include "main/EventLoop.hpp"
#include "main/WakeUp.hpp"

#include <sys/eventfd.h>

using namespace Chaining::net;

WakeUp::WakeUp(EventLoop* loop)
    : fd_ { ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK) }
    , channel_ { loop, fd_ }
{
    if (fd_ < 0) {
        ChainLogFatal("::eventfd create failed: {}", ::strerror(errno));
        std::terminate();
    }
    channel_.setReadCallback([this](Time::Timestamp timestamp) { this->handleRead(); (void)timestamp; });
    channel_.enableRead();
}

void WakeUp::Signal()
{
    int64_t sig { 1 };
    while (::write(this->fd_, &sig, sizeof(int64_t)) == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)
            continue;
        ChainLogError("wakeup signal error: {}", ::strerror(errno));
    }
}

void WakeUp::handleRead()
{
    int64_t rbuf {};
    while (::read(this->fd_, &rbuf, sizeof(int64_t)) != sizeof(int64_t)) {
        if (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)
            continue;
        ChainLogError("handle Read error (Wakeup): {}", ::strerror(errno));
    }
}

WakeUp::~WakeUp()
{
    if (::close(fd_) == -1) {
        ChainLogWarning("Close eventfd error: {}", strerror(errno));
    }
}