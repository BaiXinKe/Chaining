#ifndef CHAINING_MAIN_TCPCONNECTION_HPP__
#define CHAINING_MAIN_TCPCONNECTION_HPP__

#include "auxiliary/Nocopyable.hpp"

#include "main/Callback.hpp"
#include "main/Channel.hpp"
#include "main/Handler.hpp"
#include "main/InetAddr.hpp"

#include <any>
#include <memory>

namespace Chaining::net {

class EventLoop;
class Buffer;

class TcpConnection : auxiliary::nocopyable, public std::enable_shared_from_this<TcpConnection> {
public:
    TcpConnection(EventLoop* loop, Handler fd, const InetAddr& localAddr, const InetAddr& peerAddr);

private:
    EventLoop* loop_;
    Handler fd_;

    Channel channel_;

    InetAddr localAddr_;
    InetAddr peerAddr_;
    std::unique_ptr<Buffer> inBuffer_;
    std::unique_ptr<Buffer> outBuffer_;

    const std::string name_;

    ConnectionCallback connectioncb_;
    MessageCallback messagecb_;
    WriteCompleteCallback writeCompletecb_;
    CloseCallback closecb_;

    size_t hightWaterMark_;
    HighWaterMarkCallback highWaterMarkcb_;

    std::any context_;
};

class TcpConnectionPtr;

}

#endif