#ifndef CHAINING_MAIN_EPOLLER_HPP__
#define CHAINING_MAIN_EPOLLER_HPP__

#include "auxiliary/Nocopyable.hpp"
#include "auxiliary/Timestamp.hpp"
#include "main/Handler.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

#include <sys/epoll.h>

namespace Chaining::net {

class EventLoop;
class Channel;

class Epoller : auxiliary::nocopyable {
public:
    using ActivatedChannels = std::vector<Channel*>;

    static constexpr size_t INIT_EVENT_LIST_SIZE { 64 };

    explicit Epoller(EventLoop* loop, size_t eventListSize = INIT_EVENT_LIST_SIZE);

    void wait(ActivatedChannels* activedChannels, Time::Timestamp expTime);

    void update(Channel* channel);
    void removeChannel(Channel* channel);

private:
    void fillActivatedChannels(ActivatedChannels* activatedChannels, ssize_t n);
    void updateOperation(int op, Channel* channel);
    void removeFromEpoll(int fd);

private:
    using ChannelMaps = std::unordered_map<Handler, Channel*>;
    using ActivatedEvents = std::vector<struct epoll_event>;

    EventLoop* loop_;

    int epollfd_;

    ChannelMaps channels_;
    ActivatedEvents activatedfds_;
};

using EpollPtr = std::unique_ptr<Epoller>;

};

#endif