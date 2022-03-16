#ifndef CHANNING_BASE_NOCOPYABLE_HPP__
#define CHANNING_BASE_NOCOPYABLE_HPP__

namespace Chaining {
namespace base {

    class nocopyable {
    public:
        nocopyable(const nocopyable& other) = delete;
        nocopyable& operator=(const nocopyable&) = delete;
    };

} // base
} // Channing

#endif