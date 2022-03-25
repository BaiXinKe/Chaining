#ifndef CHAINING_MAIN_CALLBACK_HPP__
#define CHAINING_MAIN_CALLBACK_HPP__

#include "auxiliary/Timestamp.hpp"

#include "main/Handler.hpp"
#include "main/InetAddr.hpp"

#include <functional>

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
}

};

#endif