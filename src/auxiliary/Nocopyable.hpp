#ifndef CHAINING_AUXILIARY_NOCOPYABLE_HPP__
#define CHAINING_AUXILIARY_NOCOPYABLE_HPP__

namespace Chaining {

namespace auxiliary {

    class nocopyable {
    public:
        nocopyable(nocopyable const&) = delete;
        nocopyable& operator=(nocopyable const&) = delete;

    protected:
        nocopyable() = default;
        ~nocopyable() = default;
    };

}
}

#endif