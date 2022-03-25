#include "main/InetAddr.hpp"

#include "auxiliary/ErrorMsg.hpp"
#include "auxiliary/Logger.hpp"

#include <arpa/inet.h>
#include <string_view>

using namespace Chaining::net;

sockaddr_in* InetAddr::GetAsSockaddrIn()
{
    return reinterpret_cast<sockaddr_in*>(&ss_);
}

sockaddr_in6* InetAddr::GetAsSockaddrIn6()
{
    return reinterpret_cast<sockaddr_in6*>(&ss_);
}

InetAddr::InetAddr(std::string_view ip, uint16_t port, InetType type)
    : type_ { type }
{
    if (type == InetType::IPv4) {
        GetAsSockaddrIn()->sin_family = AF_INET;
        GetAsSockaddrIn()->sin_port = ::htons(port);
        if (inet_pton(AF_INET, ip.data(), &GetAsSockaddrIn()->sin_addr) == -1) {
            ChainLogError(GetErrorMsg(errno));
            std::terminate();
        }
    } else {
        GetAsSockaddrIn6()->sin6_family = AF_INET6;
        GetAsSockaddrIn6()->sin6_port = ::htons(port);
        if (inet_pton(AF_INET6, ip.data(), &GetAsSockaddrIn6()->sin6_addr) == -1) {
            ChainLogError(GetErrorMsg(errno));
            exit(EXIT_FAILURE);
        }
    }
}

InetAddr::InetAddr(std::string_view domain, InetType type)
    : type_ { type }
{
    auto lastColon = domain.find_last_of(":");
    std::string ip;
    uint16_t port;
    if (type == InetType::IPv4) {
        ip = std::string(domain.substr(0, lastColon - 1));
        port = std::atoi(domain.substr(lastColon + 1).data());

        GetAsSockaddrIn()->sin_family = AF_INET;
        GetAsSockaddrIn()->sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &GetAsSockaddrIn()->sin_addr) == -1) {
            ChainLogError(GetErrorMsg(errno));
            std::terminate();
        }

    } else {
        auto rightBracket = domain.find("]");

        assert(rightBracket < lastColon);
        (void)rightBracket;

        ip = std::string(domain.substr(1, lastColon - 2));
        port = std::atoi(domain.substr(lastColon + 1).data());

        GetAsSockaddrIn6()->sin6_family = AF_INET6;
        GetAsSockaddrIn6()->sin6_port = htons(port);
        if (inet_pton(AF_INET6, ip.c_str(), &GetAsSockaddrIn6()->sin6_addr) == -1) {
            ChainLogError(GetErrorMsg(errno));
            std::terminate();
        }
    }
}

InetAddr::InetAddr(uint32_t ip, uint16_t port)
    : type_ { InetType::IPv4 }
{
    GetAsSockaddrIn()->sin_family = AF_INET;
    GetAsSockaddrIn()->sin_port = htons(port);
    GetAsSockaddrIn()->sin_addr.s_addr = htonl(ip);
}

InetAddr::InetAddr(const sockaddr& addr, socklen_t addrlen)
{
    ::memcpy(&ss_, &addr, addrlen);
    if (addrlen == sizeof(sockaddr_in))
        type_ = InetType::IPv4;
    else
        type_ = InetType::IPv6;
}

void InetAddr::setInetAddr(const sockaddr* addr, socklen_t addrlen)
{
    if (addrlen == sizeof(sockaddr_in)) {
        type_ = InetType::IPv4;
    } else {
        type_ = InetType::IPv6;
    }

    ::memcpy(&ss_, addr, addrlen);
}

sockaddr* InetAddr::GetSockaddr()
{
    return const_cast<sockaddr*>(
        const_cast<const InetAddr*>(this)->GetSockaddr());
}

const sockaddr* InetAddr::GetSockaddr() const
{
    return reinterpret_cast<const sockaddr*>(&ss_);
}

socklen_t InetAddr::GetSize() const
{
    if (type_ == InetType::IPv4) {
        return sizeof(sockaddr_in);
    } else {
        return sizeof(sockaddr_in6);
    }
}

std::string InetAddr::toIpPort() const
{
    char ip[NI_MAXHOST] {}, port[NI_MAXSERV] {};

    if (getnameinfo(this->GetSockaddr(), this->GetSize(), ip, NI_MAXHOST, port, NI_MAXSERV, 0) == -1) {
        ChainLogWarning(GetErrorMsg(errno));
    }

    return std::string(ip) + ": " + std::string(port);
}