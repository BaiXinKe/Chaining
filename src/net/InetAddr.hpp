#ifndef CHAINING_NET_INETADDR_HPP__
#define CHAINING_NET_INETADDR_HPP__

#include <netdb.h>

namespace Chaining {
namespace net {

    class InetAddr {
    public:
        enum class Type {
            IPv4,
            IPv6,
            Invaild
        };

    public:
        InetAddr();

    private:
        sockaddr_storage ss_;
        Type type_;
    };

}
}

#endif