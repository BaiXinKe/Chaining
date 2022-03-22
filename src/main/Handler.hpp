#ifndef CHAINING_MAIN_HANDLER_HPP__
#define CHAINING_MAIN_HANDLER_HPP__

namespace Chaining {
namespace net {

#ifdef _WIN32
    using Handler = handler;
#elif defined(__linux__)
    using Handler = int;
#endif

}
}

#endif