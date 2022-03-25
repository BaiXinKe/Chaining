#ifndef CHAINING_MAIN_INETADDR_HPP__
#define CHAINING_MAIN_INETADDR_HPP__

#include <netdb.h>

#include <string_view>

namespace Chaining::net {

enum class InetType {
    IPv4,
    IPv6
};

class InetAddr {
public:
    InetAddr(std::string_view ip, uint16_t port, InetType type = InetType::IPv4);
    explicit InetAddr(std::string_view domain, InetType type = InetType::IPv4);
    InetAddr(uint32_t ip, uint16_t port);
    InetAddr(const sockaddr& addr, socklen_t addrlen);

    InetAddr() = default;

    void setInetAddr(const sockaddr* addr, socklen_t addrlen);

    sockaddr* GetSockaddr();
    const sockaddr* GetSockaddr() const;

    socklen_t GetSize() const;

    InetType type() const { return type_; }

    std::string toIpPort() const;

private:
    sockaddr_in* GetAsSockaddrIn();
    sockaddr_in6* GetAsSockaddrIn6();

private:
    sockaddr_storage ss_;
    InetType type_;
};
}

#endif