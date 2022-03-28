#ifndef CHAINING_MAIN_SOCKET_HPP__
#define CHAINING_MAIN_SOCKET_HPP__

#include "auxiliary/Nocopyable.hpp"
#include "main/Handler.hpp"

namespace Chaining::net {

class Socket : auxiliary::nocopyable {
public:
    Socket(int family, int sock_type, int protocol, bool noblocking);
    Socket(Handler handler);
    bool isNoblocking() const { return isNoblocking_; }
    void setNoblocking();

protected:
    Handler GetHandler() const;

private:
    Handler handler_;
    bool isNoblocking_;
};

}

#endif