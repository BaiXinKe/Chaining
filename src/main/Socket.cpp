#include "auxiliary/Logger.hpp"

#include "main/Socket.hpp"

#include <sys/fcntl.h>
#include <sys/socket.h>

using namespace Chaining::net;

Socket::Socket(int family, int sock_type, int protocol, bool noblocking)
    : isNoblocking_ { noblocking }
{
    sock_type |= SOCK_CLOEXEC;
    if (noblocking) {
        sock_type |= (SOCK_NONBLOCK);
    }
    handler_ = ::socket(family, sock_type, protocol);
}

Socket::Socket(Handler handler)
    : handler_ { handler }
{
    int flag = ::fcntl(handler, F_GETFD);
    flag |= FD_CLOEXEC;
    if (::fcntl(handler_, F_SETFD, flag) == -1) {
        ChainLogError("::fcntl set FD_CLOEXEC error: {}", strerror(errno));
        std::terminate();
    }
}

void Socket::setNoblocking()
{
    if (isNoblocking_)
        return;

    int flag = ::fcntl(handler_, F_GETFL);
    flag |= O_NONBLOCK;
    if (::fcntl(handler_, F_SETFL, flag) == -1) {
        ChainLogError("::fcntl set noblocking error: {}", ::strerror(errno));
        std::terminate();
    }

    isNoblocking_ = true;
}

Handler Socket::GetHandler() const
{
    return handler_;
}
