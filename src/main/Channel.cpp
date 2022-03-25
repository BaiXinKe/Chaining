#include "main/Channel.hpp"
#include "main/EventLoop.hpp"

#include "auxiliary/Timestamp.hpp"

using namespace Chaining::net;

Channel::Channel(EventLoop* loop, Handler fd)
    : loop_ { loop }
    , fd_ { fd }
    , event_ { 0 }
    , revent_ { 0 }
    , tied_ { false }
{
}

void Channel::update()
{
    this->loop_->update(this);
}

void Channel::handleEvent()
{
    if ((revent_ & EPOLLHUP) && !(revent_ & EPOLLIN)) {
        if (closecb_) {
            closecb_(Time::Now());
        }
    }

    if ((revent_ & EPOLLERR)) {
        if (errorcb_)
            errorcb_();
    }

    if ((revent_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))) {
        if (readcb_)
            readcb_(Time::Now());
    }

    if (revent_ & EPOLLOUT) {
        if (writecb_)
            writecb_();
    }
}

void Channel::remove()
{
    this->loop_->removeChannel(this);
}

Channel::~Channel()
{
    this->disableAll();
    this->remove();
}