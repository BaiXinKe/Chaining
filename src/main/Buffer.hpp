#ifndef CHAINING_MAIN_BUFFER_HPP__
#define CHAINING_MAIN_BUFFER_HPP__

#include "auxiliary/Nocopyable.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace Chaining {
namespace net {

    class Buffer : auxiliary::nocopyable {
    public:
        static constexpr size_t INIT_BUFFER_SIZE { 1024 };
        static constexpr size_t DEFAULT_PREPEND_SIZE { 64 };
        static constexpr double EXPAND_FACTOR { 2.0 };

        explicit Buffer(size_t initContainerSize = INIT_BUFFER_SIZE,
            double expandFactor = EXPAND_FACTOR, size_t prependSize = DEFAULT_PREPEND_SIZE);

        size_t readableSize() const
        {
            return writeStartPosition_ - readStartPosition_;
        }
        size_t prependableSize() const
        {
            return readStartPosition_;
        }

        const char* Peek() const
        {
            return &buff_[readStartPosition_];
        }

        char* Peek()
        {
            return const_cast<char*>(
                const_cast<const Buffer*>(this)->Peek());
        }

        void retrieve(size_t size)
        {
            readStartPosition_ += size;
        }

        void retrieveUtil(const char* pos);

        std::string retrieveAsString(size_t size);
        std::string retrieveAsStringUtil(const char* pos);

        const char* findCRLF() const;
        char* findCRLF();

        const char* findCRLFDouble() const;
        char* findCRLFDouble();

        void append(std::string_view str);
        void append(const void* str, size_t len);

    private:
        const char* begin() const
        {
            return &buff_[readStartPosition_];
        }

        char* begin()
        {
            return const_cast<char*>(
                const_cast<const Buffer*>(this)->begin());
        }

        const char* begin2Write() const
        {
            return &buff_[writeStartPosition_];
        }

        char* begin2Write()
        {
            return const_cast<char*>(
                const_cast<const Buffer*>(this)->begin2Write());
        }

        /**
         * |*******|*******|************************|***********************|
         * 0.....eps....readSPos.................writeSpos...............buff_.size()
         */
        size_t canWriteableDirection() const
        {
            return buff_.size() - writeStartPosition_;
        }

        size_t canWriteableTroughtReforming() const
        {

            return canWriteableDirection() + readStartPosition_ - easyPrependSpace_;
        }

        void reformingBuffer();

        void ensureCapacity(size_t size);

    private:
        using ContentContainer = std::vector<char>;
        ContentContainer buff_;
        size_t readStartPosition_;
        size_t writeStartPosition_;
        const size_t easyPrependSpace_;
        double expandFactor_;
    };

}
}

#endif