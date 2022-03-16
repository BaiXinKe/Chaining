#ifndef CHAINING_BASE_BUFFER_HPP__
#define CHAINING_BASE_BUFFER_HPP__

#include <vector>

namespace Chaining {
namespace base {

    class Buffer {
    public:
        static constexpr size_t DEFAULT_START_PREPEND_SIZE { 64 };
        explicit Buffer(size_t prepend_size = DEFAULT_START_PREPEND_SIZE);

    private:
        std::vector<char> buf_;
        size_t currReadIndex_;
        size_t currWriteIndex_;
        size_t prependIndex_;
    };

}
}

#endif