#ifndef CHAINING_MAIN_CHANNEL_HPP__
#define CHAINING_MAIN_CHANNEL_HPP__

#include "auxiliary/Nocopyable.hpp"

#include "main/Callback.hpp"
#include "main/Event.hpp"
#include "main/Handler.hpp"

#include <memory>

namespace Chaining::net {

class EventLoop;

class Channel : auxiliary::nocopyable {
public:
    Channel(EventLoop* loop, Handler fd);

    int getEvent() const { return event_; }
    int getRevent() const { return revent_; }
    void setRevent(int revent) { this->revent_ = revent; }
    int fd() const { return fd_; }

    void enableRead()
    {
        this->isReading_ = true;
        event_ |= static_cast<int>(Event::IN);
        this->update();
    }

    void enableWrite()
    {
        this->isWriting_ = true;
        event_ |= static_cast<int>(Event::OUT);
        this->update();
    }

    void enableError()
    {
        event_ |= static_cast<int>(Event::ERROR);
        this->update();
    }

    void disableReading()
    {
        this->isReading_ = false;
        event_ &= ~static_cast<int>(Event::IN);
        this->update();
    }

    void disableWriting()
    {
        this->isWriting_ = false;
        event_ &= ~static_cast<int>(Event::OUT);
        this->update();
    }

    void disableError()
    {
        event_ &= ~static_cast<int>(Event::ERROR);
        this->update();
    }

    void disableAll()
    {
        event_ = 0;
        this->isWriting_ = this->isReading_ = this->isErroring_ = false;
        this->update();
    }

    bool isReading() const { return isReading_; }
    bool isWriting() const { return isWriting_; }
    bool isErroring() const { return isErroring_; }

    void handleEvent();

    void tie(std::shared_ptr<void>& t)
    {
        tie_ = t;
        tied_ = true;
    }

    void setReadCallback(ReadEventCallback cb)
    {
        this->readcb_ = std::move(cb);
    }

    void setWriteCallback(WriteEventCallback cb)
    {
        this->writecb_ = std::move(cb);
    }

    void setErrorCallback(ErrorEventCallback cb)
    {
        this->writecb_ = std::move(cb);
    }

    void setCloseCallback(CloseCallback cb)
    {
        this->closecb_ = std::move(cb);
    }

    ~Channel();

private:
    void remove();
    void update();

private:
    EventLoop* loop_;

    int event_;
    int revent_;

    bool isReading_;
    bool isWriting_;
    bool isErroring_;

    std::weak_ptr<void> tie_;
    bool tied_;

    ReadEventCallback readcb_;
    WriteEventCallback writecb_;
    ErrorEventCallback errorcb_;
    CloseCallback closecb_;

    Handler fd_;
};

}

#endif