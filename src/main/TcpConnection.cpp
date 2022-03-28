#include "main/TcpConnection.hpp"
#include "main/Buffer.hpp"

using namespace Chaining::net;

TcpConnection::TcpConnection(EventLoop* loop, Handler fd, const InetAddr& localAddr, const InetAddr& peerAddr)
    : loop_ { loop }
    , fd_ { fd }
    , channel_ { loop, fd }
    , localAddr_ { localAddr }
    , peerAddr_ { peerAddr }
    , inBuffer_ { std::make_unique<Buffer>() }
    , outBuffer_ { std::make_unique<Buffer>() }
{
}
