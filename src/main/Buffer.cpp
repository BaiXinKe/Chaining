#include "main/Buffer.hpp"
#include "auxiliary/Logger.hpp"

#include <cassert>
#include <cstring>
#include <sys/uio.h>

using namespace Chaining::net;

constexpr size_t Buffer::INIT_BUFFER_SIZE;
constexpr size_t Buffer::DEFAULT_PREPEND_SIZE;
constexpr double Buffer::EXPAND_FACTOR;

Buffer::Buffer(size_t initContainerSize, double expandFactor, size_t prependSize)
    : buff_(initContainerSize, '\0')
    , readStartPosition_ { prependSize }
    , writeStartPosition_ { prependSize }
    , easyPrependSpace_ { prependSize }
    , expandFactor_ { expandFactor }
{
}

void Buffer::append(std::string_view str)
{
    this->append(str.data(), str.size());
}

void Buffer::append(const void* str, size_t size)
{
    this->ensureCapacity(size);

    const char* cStr = static_cast<const char*>(str);
    ::memcpy(begin2Write(), str, size);
    this->writeStartPosition_ += size;
}

void Buffer::ensureCapacity(size_t size)
{
    if (canWriteableDirection() >= size)
        return;

    if (canWriteableTroughtReforming() >= size) {
        reformingBuffer();
        return;
    }
    reformingBuffer();
    size_t needed { size - canWriteableDirection() };
    size_t factorExpand { static_cast<size_t>(this->buff_.size() * expandFactor_) };

    size_t realExpandSize { std::max(needed, factorExpand) };
    buff_.resize(realExpandSize);
}

void Buffer::reformingBuffer()
{
    size_t canReadSize { this->readableSize() };

    ::memmove(&buff_[easyPrependSpace_], &buff_[readStartPosition_], canReadSize);

    this->readStartPosition_ = easyPrependSpace_;
    this->writeStartPosition_ = easyPrependSpace_ + canReadSize;
}

void Buffer::retrieveUtil(const char* pos)
{
    assert(pos > this->begin() && pos < this->begin2Write());
    size_t retrieveSize { pos - this->begin() };
    this->retrieve(retrieveSize);
}

std::string Buffer::retrieveAsString(size_t size)
{
    if (readableSize() < size)
        return {};

    std::string str { this->Peek(), size };
    this->retrieve(size);
    return str;
}

std::string Buffer::retrieveAsStringUtil(const char* pos)
{
    assert(pos > this->begin() && pos < this->begin2Write());
    size_t retrieveSize { pos - this->begin() };
    return this->retrieveAsString(retrieveSize);
}

const char* Buffer::findCRLF() const
{
    const char* pos = std::search(this->begin(), this->begin2Write(), "\r\n");
    return pos != this->begin2Write() ? pos : nullptr;
}

char* Buffer::findCRLF()
{
    return const_cast<char*>(
        const_cast<const Buffer*>(this)->findCRLF());
}

const char* Buffer::findCRLFDouble() const
{
    const char* pos = std::search(this->begin(), this->begin2Write(), "\r\n\r\n");
    return pos != this->begin2Write() ? pos : nullptr;
}

char* Buffer::findCRLFDouble()
{
    return const_cast<char*>(
        const_cast<const Buffer*>(this)->findCRLFDouble());
}

ssize_t Buffer::readFromFD(Handler fd)
{
    static char tmpBuff[65536] {};
    struct iovec iov[2] {};
    iov[0].iov_base = this->begin2Write();
    iov[0].iov_len = this->canWriteableDirection();
    iov[1].iov_base = tmpBuff;
    iov[1].iov_len = sizeof(tmpBuff);

    ssize_t iovReadRet { ::readv(fd, iov, 2) };
    if (iovReadRet < 0) {
        ChainLogError("::readv error: {}", std::string(strerror(errno)));
        return -1;
    }

    if (iovReadRet > this->canWriteableDirection()) {
        size_t overRead { iovReadRet - this->canWriteableDirection() };
        this->writeStartPosition_ = this->buff_.size();
        this->append(tmpBuff, overRead);
    } else {
        this->writeStartPosition_ += iovReadRet;
    }

    return iovReadRet;
}