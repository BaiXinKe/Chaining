#ifndef CHAINING_MAIN_EVENT_HPP__
#define CHAINING_MAIN_EVENT_HPP__

#include <sys/epoll.h>

namespace Chaining::net {

enum class Event : int {
    IN = EPOLLIN | EPOLLHUP,
    OUT = EPOLLOUT,
    ERROR = EPOLLERR
};

}

#endif