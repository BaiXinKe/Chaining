#include "auxiliary/Logger.hpp"
#include "auxiliary/Timestamp.hpp"

#include "main/Channel.hpp"
#include "main/Epoller.hpp"
#include "main/EventLoop.hpp"

#include <string>

using namespace Chaining::net;

namespace {
static std::string op2String(int op)
{
    switch (op) {
    case EPOLL_CTL_ADD:
        return "EPOLL_CTL_ADD";
    case EPOLL_CTL_MOD:
        return "EPOLL_CTL_MOD";
    case EPOLL_CTL_DEL:
        return "EPOLL_CTL_DEL";
    }
    return "(UNKNOWN OP)";
}
}

Epoller::Epoller(EventLoop* loop, size_t eventListSize)
    : loop_ { loop }
    , epollfd_ { ::epoll_create1(EPOLL_CLOEXEC) }
    , activatedfds_(eventListSize)
{
    if (epollfd_ < 0) {
        ChainLogFatal("::epoll_create error because: {}",
            ::strerror(errno));
        std::terminate();
    }
}

void Epoller::wait(ActivatedChannels* activatedChannels, Time::Timestamp expTime)
{
    this->loop_->assertInLoopThread();
    int64_t exp { -1 };
    if (expTime != Time::Timestamp()) {
        exp = Time::GetMillsecondsFromNow(expTime);
    }

    ssize_t nret = ::epoll_wait(this->epollfd_, &activatedfds_[0], activatedfds_.size(), exp);
    if (nret < 0) {
        if (errno != EINTR) {
            ChainLogError("epoll_wait error happend: {}", ::strerror(errno));
        }
        return;
    }

    this->fillActivatedChannels(activatedChannels, nret);

    if (nret == activatedfds_.size()) {
        activatedfds_.resize(activatedfds_.size() * 2);
    }
}

void Epoller::fillActivatedChannels(ActivatedChannels* activatedChannels, ssize_t n)
{
    for (int i = 0; i < n; i++) {
        Channel* channel = static_cast<Channel*>(activatedfds_[i].data.ptr);
        assert(channels_.find(channel->fd()) != channels_.end());
        activatedChannels->push_back(channel);
    }
}

void Epoller::update(Channel* channel)
{
    loop_->assertInLoopThread();
    int op {};

    if (channels_.find(channel->fd()) == std::end(channels_)) {
        ChainLogTrace("New Channel Registe in the epoll instance fd: {}", channel->fd());
        op = EPOLL_CTL_ADD;
        channels_[channel->fd()] = channel;
    } else {
        ChainLogTrace("Channel Listening event changed: {}->{}", channel->fd(), channel->getEvent());
        op = EPOLL_CTL_MOD;
    }

    updateOperation(op, channel);
}

void Epoller::removeChannel(Channel* channel)
{
    loop_->assertInLoopThread();
    assert(channels_.find(channel->fd()) != channels_.end());
    size_t n = channels_.erase(channel->fd());
    assert(n == 1);
    this->removeFromEpoll(channel->fd());
}

void Epoller::updateOperation(int op, Channel* channel)
{
    epoll_event event;
    event.data.ptr = channel;
    event.events = channel->getEvent();
    if (::epoll_ctl(this->epollfd_, op, channel->fd(), &event) == -1) {
        ChainLogError("epoll_ctl({}) error because: fd: {}->{}", op2String(op), channel->fd(), strerror(errno));
    }
}

void Epoller::removeFromEpoll(int fd)
{
    if (::epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        ChainLogWarning("epoll_ctl(EPOLL_CTL_DEL) error because: fd: {}->{}", fd, strerror(errno));
    }
}
