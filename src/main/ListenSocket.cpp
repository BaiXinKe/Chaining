#include "main/ListenSocket.hpp"
#include "main/Channel.hpp"
#include "main/EventLoop.hpp"
#include "main/InetAddr.hpp"
#include "main/Socket.hpp"

#include "auxiliary/ErrorMsg.hpp"
#include "auxiliary/Logger.hpp"

using namespace Chaining::net;

ListenSocket::ListenSocket(EventLoop* loop, int family, int sock_type, int protocol)
    : Socket { family, sock_type, protocol, true }
    , loop_ { loop }
    , idlefd_ { ::open("/dev/null", O_RDWR | O_CLOEXEC) }
{
    this->setReuseAddr();
    this->setReusePort();
    channel_.reset(new Channel(loop, this->GetHandler()));
    channel_->setReadCallback([this](Time::Timestamp ts) {
        this->accept(ts);
    });
    channel_->enableRead();
}

void ListenSocket::setReuseAddr()
{
    loop_->assertInLoopThread();
    int on { 1 };
    if (::setsockopt(this->GetHandler(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        ChainLogError("::setsocketopt (SO_REUSEADDR) error: {}", strerror(errno));
        std::terminate();
    }
}

void ListenSocket::setReusePort()
{
    loop_->assertInLoopThread();
    int on { 1 };
    if (::setsockopt(this->GetHandler(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) == -1) {
        ChainLogError("::setsocketopt (SO_REUSEPORT) error: {}", strerror(errno));
        std::terminate();
    }
}

void ListenSocket::bind(const InetAddr& addr)
{
    loop_->assertInLoopThread();
    if (::bind(this->GetHandler(), addr.GetSockaddr(), addr.GetSize()) == -1) {
        ChainLogFatal(GetErrorMsg(errno));
        std::terminate();
    }
}

void ListenSocket::listen(int linq)
{
    loop_->assertInLoopThread();
    if (::listen(this->GetHandler(), linq) == -1) {
        ChainLogFatal(GetErrorMsg(errno));
        std::terminate();
    }
}

void ListenSocket::accept(Time::Timestamp recevieTime)
{
    loop_->assertInLoopThread();

    sockaddr_storage ss_;
    socklen_t len = sizeof(ss_);

    Handler ret {};
    if ((ret = ::accept4(this->GetHandler(), (sockaddr*)&ss_, &len, SOCK_NONBLOCK | SOCK_CLOEXEC)) == -1) {
        if (errno == EMFILE) {
            ::close(idlefd_);
            idlefd_ = ::accept(this->GetHandler(), NULL, NULL);
            ::close(idlefd_);
            idlefd_ = ::open("/dev/null", O_RDWR | O_CLOEXEC);
        }

        ChainLogWarning(GetErrorMsg(errno));
        return;
    }

    ChainLogInfo("New Connection");

    (void)recevieTime;
    InetAddr inetaddr { *reinterpret_cast<const sockaddr*>(&ss_), len };
    if (newConnectioncb_) {
        newConnectioncb_(ret, inetaddr);
    } else {
        ::close(ret);
    }
}

ListenSocket::~ListenSocket()
{
    ::close(idlefd_);
}