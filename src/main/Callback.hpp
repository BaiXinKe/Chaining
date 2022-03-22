#ifndef CHAINING_MAIN_CALLBACK_HPP__
#define CHAINING_MAIN_CALLBACK_HPP__

#include <functional>

namespace Chaining {

namespace Time {
    using TimerCallback = std::function<void()>;

}

};

#endif