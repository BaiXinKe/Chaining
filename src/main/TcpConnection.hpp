#ifndef CHAINING_MAIN_TCPCONNECTION_HPP__
#define CHAINING_MAIN_TCPCONNECTION_HPP__

#include "auxiliary/Nocopyable.hpp"

#include "main/Callback.hpp"
#include "main/Channel.hpp"
#include "main/Handler.hpp"
#include "main/InetAddr.hpp"
#include "main/Socket.hpp"

#include <any>
#include <memory>

namespace Chaining::net {

class EventLoop;
class Buffer;

class TcpConnection : auxiliary::nocopyable, public Socket, public std::enable_shared_from_this<TcpConnection> {
public:
    enum class State {
        Connecting,
        Connected,
        Disconnecting,
        Disconnected
    };

    TcpConnection(EventLoop* loop, Handler fd, const InetAddr& localAddr,
        const InetAddr& peerAddr, std::string_view name);

    void send(const void* data, size_t len);
    void send(std::string_view data);
    void send(Buffer* buff);

    void sendInLoop(std::string_view data);
    void sendInLoop(const void* data, size_t len);

    void connectEstablished();

    void connectDestroyed();

    void shutdown();
    void shutdownInLoop();

    void forceClose();
    void forceCloseWithDelay(double seconds);
    void forceCloseInLoop();

    void setTcpNoDelay(bool on);

    void startRead();
    void startReadInLoop();
    void stopRead();
    void stopReadInLoop();

    bool connected() const
    {
        return this->state_ == State::Connected;
    }

    void setContext(const std::any& context)
    {
        this->context_ = context;
    }

    const std::any& getContext() const
    {
        return this->context_;
    }

    std::any* getMultiableContext()
    {
        return &this->context_;
    }

    void setConnectionCallback(ConnectionCallback cb)
    {
        connectioncb_ = cb;
    }

    void setMessageCallback(MessageCallback cb)
    {
        messagecb_ = cb;
    }

    void setWriteCompeleteCallback(WriteCompleteCallback cb)
    {
        writeCompletecb_ = cb;
    }

    void setCloseCallback(CloseCallback cb)
    {
        closecb_ = cb;
    }

    const InetAddr* getLocalAddr() const
    {
        return &localAddr_;
    }

    const InetAddr* getPeerAddr() const
    {
        return &peerAddr_;
    }

    EventLoop* getLoop()
    {
        return loop_;
    }
    std::string name() const
    {
        return name_;
    }

    ~TcpConnection();

private:
    State state_;

    EventLoop* loop_;

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