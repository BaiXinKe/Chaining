#ifndef CHAINING_MAIN_CALLBACK_HPP__
#define CHAINING_MAIN_CALLBACK_HPP__

#include "auxiliary/Timestamp.hpp"

#include "main/Handler.hpp"
#include "main/InetAddr.hpp"

#include <functional>
#include <memory>

namespace Chaining {

namespace Time {
    using TimerCallback = std::function<void()>;
}

namespace net {
    using ReadEventCallback = std::function<void(Time::Timestamp)>;
    using WriteEventCallback = std::function<void()>;
    using ErrorEventCallback = std::function<void()>;
    using CloseCallback = std::function<void(Time::Timestamp)>;

    using PendingTask = std::function<void()>;

    using NewConnectionCallback = std::function<void(Handler fd, const InetAddr&)>;

    // TcpConnection
    class TcpConnection;
    class Buffer;
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
    using CloseCallback = std::function<void(const TcpConnectionPtr&)>;
    using WriteCompleteCallback = std::function<void(const TcpConnectionPtr&)>;
    using HighWaterMarkCallback = std::function<void(const TcpConnectionPtr&, size_t)>;

    using MessageCallback = std::function<void(const TcpConnectionPtr&, Buffer*, Time::Timestamp)>;

    void defaultConnectionCallBack(const TcpConnectionPtr& conn);
    void defaultMessageCallback(const TcpConnectionPtr&, Buffer*, Time::Timestamp);
}

};

#endif