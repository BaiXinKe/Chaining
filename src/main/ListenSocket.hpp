#ifndef CHAINING_MAIN_LISTENSOCKET_HPP__
#define CHAINING_MAIN_LISTENSOCKET_HPP__

#include "main/Callback.hpp"
#include "main/Socket.hpp"

#include <memory>
#include <sys/socket.h>

namespace Chaining::net {

class InetAddr;
class Channel;
class EventLoop;

class ListenSocket : public Socket {
public:
    ListenSocket(EventLoop* loop, int family, int sock_type, int protocol);

    void setReuseAddr();
    void setReusePort();

    void bind(const InetAddr& addr);
    void listen(int linq = SOMAXCONN);

    void setNewConnectionCallback(NewConnectionCallback cb)
    {
        newConnectioncb_ = cb;
    }

    ~ListenSocket();

private:
    void accept(Time::Timestamp recevieTime);

private:
    EventLoop* loop_;
    std::unique_ptr<Channel> channel_;
    NewConnectionCallback newConnectioncb_;
    Handler idlefd_;
};

}

#endif