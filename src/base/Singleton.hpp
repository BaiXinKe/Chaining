#ifndef CHAINING_BASE_SINGLETON_HPP__
#define CHAINING_BASE_SINGLETON_HPP__

#include "base/nocopyable.hpp"
#include <utility>

namespace Chaining {
namespace base {

    template <typename InstanceType>
    class Singleton : nocopyable {
    public:
        template <typename... Args>
        Singleton& getInstance(Args&&... args)
        {
            static InstanceType instance_ { std::forward<Args>(args)... };
            return instance_;
        }

    protected:
        Singleton() = default;
        ~Singleton() = default;
    };

}
}

#endif