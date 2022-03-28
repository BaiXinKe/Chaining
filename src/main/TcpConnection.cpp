#include "main/TcpConnection.hpp"
#include "main/Buffer.hpp"

using namespace Chaining::net;

TcpConnection::TcpConnection(EventLoop* loop, Handler fd, const InetAddr& localAddr,
    const InetAddr& peerAddr, std::string_view name)
    : Socket { fd }
    , state_ { State::Connecting }
    , loop_ { loop }
    , channel_ { loop, fd }
    , localAddr_ { localAddr }
    , peerAddr_ { peerAddr }
    , inBuffer_ { std::make_unique<Buffer>() }
    , outBuffer_ { std::make_unique<Buffer>() }
    , name_ { name }
{
    this->setNoblocking();
}

TcpConnection::~TcpConnection()
{
}